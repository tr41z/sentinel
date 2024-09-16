use std::net::Ipv4Addr;

use pnet::packet::ip::IpNextHeaderProtocol;

use crate::utils::flow::Flow;

#[allow(unused)]
pub fn handle_packet_flow
(
    src_ip: Ipv4Addr,
    dst_ip: Ipv4Addr,
    src_port: u16,
    dst_port: u16,
    protocol: IpNextHeaderProtocol,
    size: u16
) -> Flow {
    // let flow: Flow = Flow::new(all above vars);
    // let flow_key: FlowKey = FlowKey::new(only first five vars);

    /* Using HashMap (and possibly Mutex) for flow aggregation and updates.*/

    Flow {}
}