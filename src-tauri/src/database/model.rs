use std::{net::Ipv4Addr, time::SystemTime};

#[derive(Debug, sqlx::FromRow)]
pub struct DataModel {
    pub src_ip: Ipv4Addr, pub dst_ip: Ipv4Addr,
    pub src_port: u16, pub dst_port: u16,
    pub protocol: u8,
    pub total_bytes: u64,
    pub total_packet_count: u32,
    pub start_time: SystemTime,
    pub end_time: Option<SystemTime>,
}

impl DataModel {
    pub fn new(
        src_ip: Ipv4Addr, dst_ip: Ipv4Addr,
        src_port: u16, dst_port: u16,
        protocol: u8,
        total_bytes: u64,
        total_packet_count: u32,
        start_time: SystemTime,
        end_time: Option<SystemTime>,
    ) -> Self {
        return Self {
           src_ip, dst_ip,
           src_port, dst_port,
           protocol,
           total_bytes,
           total_packet_count,
           start_time, end_time
        }
    }
}