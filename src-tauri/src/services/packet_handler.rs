#[allow(unused)]
use std::{collections::HashMap, net::Ipv4Addr, sync::Mutex};

use pnet::packet::ip::IpNextHeaderProtocol;

use crate::utils::flow::{Flow, FlowKey};

/* REMINDER: EVERYTHING HERE IS IN LOOP */
pub fn handle_packet_flow
(
    src_ip: Ipv4Addr,
    dst_ip: Ipv4Addr,
    src_port: u16,
    dst_port: u16,
    protocol: IpNextHeaderProtocol,
    size: u16,
    flows_map: &mut HashMap<FlowKey, Flow>
) {

    let flow: Flow = Flow::new(
        src_ip, dst_ip, 
        src_port, dst_port, 
        protocol.0, size as u64
    );
    
    let flow_key: FlowKey = FlowKey::new(
        src_ip, dst_ip, 
        src_port, dst_port, protocol.0
    );

    // TODO get value hidden under key
    // TODO then if it already exists, update
    // TODO if doesn't initialise new one
    match flows_map.get_mut(&flow_key) {
        Some(flow) => {
            flow.update(size as u64);
            flow.pretty_print("Flow Updated");
        },
        None => {
            flows_map.insert(flow_key, flow);
            flow.pretty_print("New Flow");
        }
    };
}