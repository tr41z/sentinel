use pnet::packet::ethernet::EthernetPacket;
use pnet::packet::ip::{IpNextHeaderProtocol, IpNextHeaderProtocols};
use pnet::packet::ipv4::Ipv4Packet;
use pnet::packet::tcp::TcpPacket;
use pnet::packet::udp::UdpPacket;
use pnet::packet::Packet;

use std::collections::HashMap;
use std::net::Ipv4Addr;

#[allow(unused)]
use std::sync::{Arc, Mutex};

use pnet::datalink::{self, NetworkInterface};

use crate::services::capture::datalink::Channel::Ethernet;
use crate::utils::flow::{Flow, FlowKey};

use super::packet_handler::handle_packet_flow;

// NOTE: Adding Arc<Mutex<HashMap<FlowKey, Flow>>> later to eliminate risk of data races
// lazy_static::lazy_static! {
//     static ref flows_map: Arc<Mutex<HashMap<FlowKey, Flow>>> = Arc::new(Mutex::new(HashMap::new()));
// }

/* FIXME: Using HashMap (and possibly Mutex) for flow aggregation and updates. (line 20) */
pub fn capture_packets(interface: NetworkInterface) {
    let mut flows_map: HashMap<FlowKey, Flow> = HashMap::new();

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

                        handle_packet_flow(
                            // FlowKey attributes
                            src_ip,
                            dst_ip,
                            src_port,
                            dst_port,
                            protocol,
                            // Flow + FlowKey attributes
                            size,
                            // Flows map
                            &mut flows_map,
                        );
                    }
                }
            }
            Err(e) => {
                panic!("An error occurred while reading: {}", e);
            }
        }
    }
}
