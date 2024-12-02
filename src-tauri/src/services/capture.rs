use async_std::task;

use pnet::datalink::{self, NetworkInterface};
use pnet::packet::ethernet::EthernetPacket;
use pnet::packet::ip::{IpNextHeaderProtocol, IpNextHeaderProtocols};
use pnet::packet::ipv4::Ipv4Packet;
use pnet::packet::tcp::TcpPacket;
use pnet::packet::udp::UdpPacket;
use pnet::packet::Packet;

use std::collections::HashMap;
use std::net::{IpAddr, Ipv4Addr};
use std::sync::{Arc, Mutex};

use lazy_static::lazy_static;

use sqlx::{Pool, Sqlite};

use crate::database;
use crate::services::capture::datalink::Channel::Ethernet;
use crate::utils::flow::{Flow, FlowKey};

use super::packet_handler::handle_packet_flow;

lazy_static! {
    static ref LOCAL_IP: Mutex<Option<Ipv4Addr>> = Mutex::new(None);
}

// Function to get the local IP address dynamically
fn get_local_ip(interface: &NetworkInterface) -> Option<Ipv4Addr> {
    // Check if the local IP is already cached
    let mut local_ip: std::sync::MutexGuard<'_, Option<Ipv4Addr>> = LOCAL_IP.lock().unwrap();

    if local_ip.is_none() {
        // Iterate through the interface's IP addresses and return the first valid IPv4 address
        for ip_network in &interface.ips {
            if ip_network.ip().is_ipv4() {
                if let IpAddr::V4(ipv4_addr) = ip_network.ip() {
                    // Skip loopback addresses like 127.0.0.1
                    if ipv4_addr != Ipv4Addr::LOCALHOST {
                        *local_ip = Some(ipv4_addr);
                        return Some(ipv4_addr);
                    }
                }
            }
        }
    }

    // Return the cached IP if it exists
    *local_ip
}

pub fn capture_packets(interface: NetworkInterface) {
    let db: Pool<Sqlite> = match task::block_on(database::db::connect()) {
        Ok(pool) => pool,
        Err(e) => panic!("Failed to connect to the database: {}", e),
    };

    let flows_map: Arc<Mutex<HashMap<FlowKey, Flow>>> = Arc::new(Mutex::new(HashMap::new()));

    let (_, mut rx) = match datalink::channel(&interface, Default::default()) {
        Ok(Ethernet(tx, rx)) => (tx, rx),
        Ok(_) => panic!("Unhandled channel type: {}", &interface),
        Err(e) => panic!(
            "An error occurred when creating the datalink channel: {}",
            e
        ),
    };

    // Get the local IP address for the chosen interface
    let local_ip: Ipv4Addr = get_local_ip(&interface).expect("Failed to retrieve local IP address");

    loop {
        match rx.next() {
            Ok(packet) => {
                if let Some(ethernet_packet) = EthernetPacket::new(packet) {
                    if let Some(ip_packet) = Ipv4Packet::new(ethernet_packet.payload()) {
                        // Extracting IP packets attributes
                        let src_ip: Ipv4Addr = ip_packet.get_source();
                        let dst_ip: Ipv4Addr = ip_packet.get_destination();
                        let protocol: IpNextHeaderProtocol = ip_packet.get_next_level_protocol();
                        let size: u16 = ip_packet.get_total_length();
                        let ttl: u8 = ip_packet.get_ttl();

                        // Check if the destination IP is valid
                        if dst_ip.is_broadcast() || dst_ip.is_multicast() || dst_ip != local_ip {
                            continue; // Skip this packet
                        }

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
                            src_ip, dst_ip, src_port, dst_port, protocol, size, ttl, flows_map,
                            &db, local_ip,
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
