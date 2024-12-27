use std::{net::Ipv4Addr, time::SystemTime};

use serde::Serialize;

#[derive(Debug, sqlx::FromRow, Serialize)]
pub struct DataModel {
    pub src_ip: Ipv4Addr,
    pub dst_ip: Ipv4Addr,
    pub protocol: u8,
    pub total_bytes: u64,
    pub rate: f64,
    pub avg_packet_size: f64,
    pub total_packet_count: u16,
    pub src_port_count: u64,
    pub dst_port_count: u64,
    pub start_time: SystemTime,
    pub last_update_time: SystemTime,
    pub duration: u64,
}
