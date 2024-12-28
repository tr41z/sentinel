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
use log::{info, error};
use simplelog::{Config, WriteLogger, LevelFilter};
use std::fs::File;
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
    let mut local_ip: std::sync::MutexGuard<'_, Option<Ipv4Addr>> = LOCAL_IP.lock().unwrap();

    if local_ip.is_none() {
        for ip_network in &interface.ips {
            if ip_network.ip().is_ipv4() {
                if let IpAddr::V4(ipv4_addr) = ip_network.ip() {
                    if ipv4_addr != Ipv4Addr::LOCALHOST {
                        *local_ip = Some(ipv4_addr);
                        return Some(ipv4_addr);
                    }
                }
            }
        }
    }

    *local_ip
}

pub fn capture_packets(interface: NetworkInterface) {
    info!("Starting packet capture on interface: {}", interface.name);

    // Check if the interface exists and is available
    let available_interfaces = datalink::interfaces();
    if !available_interfaces.contains(&interface) {
        error!("The specified network interface does not exist or is not available: {}", interface.name);
        panic!("The specified network interface does not exist or is not available: {}", interface.name);
    }

    let db: Pool<Sqlite> = match task::block_on(database::db::connect()) {
        Ok(pool) => pool,
        Err(e) => {
            error!("Failed to connect to the database: {}", e);
            panic!("Failed to connect to the database: {}", e);
        }
    };

    let flows_map: Arc<Mutex<HashMap<FlowKey, Flow>>> = Arc::new(Mutex::new(HashMap::new()));

    let (_, mut rx) = match datalink::channel(&interface, Default::default()) {
        Ok(Ethernet(tx, rx)) => (tx, rx),
        Ok(_) => {
            error!("Unhandled channel type for interface: {}", interface.name);
            panic!("Unhandled channel type for interface: {}", interface.name);
        }
        Err(e) => {
            error!("An error occurred when creating the datalink channel on interface {}: {}", interface.name, e);
            panic!("An error occurred when creating the datalink channel on interface {}: {}", interface.name, e);
        }
    };

    // Get the local IP address for the chosen interface
    let local_ip: Ipv4Addr = match get_local_ip(&interface) {
        Some(ip) => ip,
        None => {
            error!("Failed to retrieve local IP address");
            panic!("Failed to retrieve local IP address");
        }
    };

    loop {
        match rx.next() {
            Ok(packet) => {
                if let Some(ethernet_packet) = EthernetPacket::new(packet) {
                    if let Some(ip_packet) = Ipv4Packet::new(ethernet_packet.payload()) {
                        let src_ip: Ipv4Addr = ip_packet.get_source();
                        let dst_ip: Ipv4Addr = ip_packet.get_destination();
                        let protocol: IpNextHeaderProtocol = ip_packet.get_next_level_protocol();
                        let size: u16 = ip_packet.get_total_length();
                        let ttl: u8 = ip_packet.get_ttl();

                        // Log packet information
                        info!(
                            "Received packet: src_ip = {}, dst_ip = {}, protocol = {:?}, size = {}, ttl = {}",
                            src_ip, dst_ip, protocol, size, ttl
                        );

                        if dst_ip.is_broadcast() || dst_ip.is_multicast() || dst_ip != local_ip {
                            continue; // Skip this packet
                        }

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
                            src_ip, dst_ip, src_port, dst_port, protocol, size, flows_map, &db,
                            local_ip,
                        ));
                    }
                }
            }
            Err(e) => {
                error!("An error occurred while reading: {}", e);
                panic!("An error occurred while reading: {}", e);
            }
        }
    }
}
