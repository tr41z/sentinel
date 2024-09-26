use std::{net::Ipv4Addr, time::SystemTime};

#[derive(Debug, PartialEq, Eq)]
#[allow(unused)]
pub struct DataModel {
    src_ip: Ipv4Addr, dst_ip: Ipv4Addr,
    src_port: u16, dst_port: u16,
    protocol: u8,
    total_bytes: u64,
    total_packet_count: u32,
    start_time: SystemTime,
    end_time: Option<SystemTime>,
}