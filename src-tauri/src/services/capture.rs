use async_std::task;
use pnet::packet::ethernet::EthernetPacket;
use pnet::packet::ip::{IpNextHeaderProtocol, IpNextHeaderProtocols};
use pnet::packet::ipv4::Ipv4Packet;
use pnet::packet::tcp::TcpPacket;
use pnet::packet::udp::UdpPacket;
use pnet::packet::Packet;

use std::collections::HashMap;
use std::net::Ipv4Addr;
use std::sync::{Arc, Mutex};

use sqlx::{Pool, MySql};

use pnet::datalink::{self, NetworkInterface};

use crate::database;
use crate::services::capture::datalink::Channel::Ethernet;
use crate::utils::flow::{Flow, FlowKey};

use super::packet_handler::handle_packet_flow;

pub fn capture_packets(interface: NetworkInterface) {
    let db: Pool<MySql> = match task::block_on(database::db::connect()) {
        Ok(pool) => pool,
        Err(e) => panic!("Failed to connect to the database: {}", e),
    };

    let flows_map: Arc<Mutex<HashMap<FlowKey, Flow>>> = Arc::new(Mutex::new(HashMap::new()));

    let (_, mut rx) = match datalink::channel(&interface, Default::default()) {
        Ok(Ethernet(tx, rx)) => (tx, rx),
        Ok(_) => panic!("Unhandled channel type: {}", &interface),
        Err(e) => panic!("An error occured when creating the datalink channel: {}", e),
    };

    loop {
        match rx.next() {
            Ok(packet) => {
                if let Some(ethernet_packet) = EthernetPacket::new(packet) {
                    if let Some(ip_packet) = Ipv4Packet::new(ethernet_packet.payload()) {
                        // Extracting ip packets attributes
                        let src_ip: Ipv4Addr = ip_packet.get_source();
                        let dst_ip: Ipv4Addr = ip_packet.get_destination();
                        let protocol: IpNextHeaderProtocol = ip_packet.get_next_level_protocol();
                        let size: u16 = ip_packet.get_total_length();
                        let ttl: u8 = ip_packet.get_ttl();

                        // Extracting source port and destination port from packets
                        let (src_port, dst_port) = match protocol {
                            IpNextHeaderProtocols::Tcp => {
                                if let Some(tcp_packet) = TcpPacket::new(ip_packet.payload()) {
                                    (tcp_packet.get_source(), tcp_packet.get_destination())
                                } else {
                                    continue;
                                }
                            }
                            IpNextHeaderProtocols::Udp => {
                                if let Some(udp_packet) = UdpPacket::new(ip_packet.payload()) {
                                    (udp_packet.get_source(), udp_packet.get_destination())
                                } else {
                                    continue;
                                }
                            }
                            _ => continue,
                        };

                        let flows_map: Arc<Mutex<HashMap<FlowKey, Flow>>> = Arc::clone(&flows_map);

                        task::block_on(handle_packet_flow(
                            // FlowKey attributes
                            src_ip,
                            dst_ip,
                            src_port,
                            dst_port,
                            protocol,
                            
                            // Flow + FlowKey attributes
                            size,
                            ttl,
                            
                            // Flows map
                            flows_map,
                            &db
                        ));
                    }
                }
            }
            Err(e) => {
                panic!("An error occurred while reading: {}", e);
            }
        }
    }
}
