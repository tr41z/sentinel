use tauri::{AppHandle, Manager};

use pnet::packet::ethernet::EthernetPacket;
use pnet::packet::ip::{IpNextHeaderProtocol, IpNextHeaderProtocols};
use pnet::packet::ipv4::Ipv4Packet;
use pnet::packet::tcp::TcpPacket;
use pnet::packet::udp::UdpPacket;
use pnet::packet::Packet;

use std::net::Ipv4Addr;

use pnet::datalink::{self, NetworkInterface};

use crate::services::capture::datalink::Channel::Ethernet;
use crate::utils::flow::Flow;

use super::packet_handler::handle_packet_flow;

#[allow(unused)]
pub fn capture_packets(interface: NetworkInterface, app_handle: AppHandle) {
    let (_, mut rx) = match datalink::channel(&interface, Default::default()) {
        Ok(Ethernet(tx, rx)) => (tx, rx),
        Ok(_) => panic!("Unhandled channel type: {}", &interface),
        Err(e) => panic!("An error occured when creating the datalink channel: {}", e)
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

                        let (src_port, dst_port) = match protocol {
                            IpNextHeaderProtocols::Tcp => {
                                if let Some(tcp_packet) = TcpPacket::new(ip_packet.payload()) {
                                    (tcp_packet.get_source(), tcp_packet.get_destination())
                                } else {
                                    return
                                }
                            }
                            IpNextHeaderProtocols::Udp => {
                                if let Some(udp_packet) = UdpPacket::new(ip_packet.payload()) {
                                    (udp_packet.get_source(), udp_packet.get_destination())
                                } else {
                                    return
                                }
                            }
                            _ => return
                        };

                        let flow: Flow = handle_packet_flow(
                            // FlowKey attr
                            src_ip, 
                            dst_ip, 
                            src_port, 
                            dst_port,
                            protocol,
                            
                            // Flow + FlowKey attr
                            size
                        );

                        if let Err(e) = app_handle.emit_all("pass_flow", flow) {
                            log::error!("Failed to emit flows: {}", e)
                        }
                    }
                }
            },
            Err(e) => {
                panic!("An error occurred while reading: {}", e);
            }
        }
    }
}
