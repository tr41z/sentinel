use std::{net::Ipv4Addr, time::SystemTime};

use serde::Serialize;

#[derive(Debug, sqlx::FromRow, Serialize)]
pub struct DataModel {
    pub src_ip: Ipv4Addr,
    pub dst_ip: Ipv4Addr,
    pub src_port_count: u16,
    pub dst_port_count: u16,
    pub protocol: u8,
    pub total_bytes: u64,
    pub total_packet_count: u64,
    pub start_time: SystemTime,
    pub last_update_time: SystemTime,
    pub duration: f32,
}