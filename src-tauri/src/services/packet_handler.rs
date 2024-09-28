use std::time::SystemTime;

use std::{collections::HashMap, net::Ipv4Addr};

use pnet::packet::ip::IpNextHeaderProtocol;

use crate::utils::flow::{Flow, FlowKey};
#[allow(unused)]
use crate::database::db;

// WARNING: EVERYTHING IN THAT FUNCTION IS LOOPED
pub fn handle_packet_flow(
    src_ip: Ipv4Addr,
    dst_ip: Ipv4Addr,
    src_port: u16,
    dst_port: u16,
    protocol: IpNextHeaderProtocol,
    size: u16,

    flows_map: &mut HashMap<FlowKey, Flow>,
) {
    // Cleanup terminated flows before handling new packets
    flows_map.retain(|_, flow| !flow.finished);

    let flow: Flow = Flow::new(
        src_ip,
        dst_ip,
        src_port,
        dst_port,
        protocol.0,
        size as u64,
        SystemTime::now(),
        SystemTime::now(),
        None,
    );

    let flow_key: FlowKey = FlowKey::new(src_ip, dst_ip, src_port, dst_port, protocol.0);

    // TODO: get value hidden under key
    // TODO: then if it already exists, update
    // TODO: if doesn't initialize new one
    match flows_map.get_mut(&flow_key) {
        Some(flow) => {
            // Terminate flows that were inactive for specified duration
            terminate_flows(flow);

            // Update flow only if it's not finished
            if !flow.finished {
                flow.update(size as u64, SystemTime::now());
                flow.pretty_print("Flow Updated");
            }
        }
        None => {
            flows_map.insert(flow_key, flow);
            flow.pretty_print("New Flow")
        }
    };
}

// Terminate flows that were inactive for more than 5 seconds
fn terminate_flows(flow: &mut Flow) {
    let now: SystemTime = SystemTime::now();
    
    if let Ok(duration) = now.duration_since(flow.last_update_time) {
        if duration.as_secs() >= 5 {
            flow.finished = true;
            flow.end_time = Some(now);
            flow.flow_termination_print();
        }
    }
}