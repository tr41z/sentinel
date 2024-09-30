use std::{net::Ipv4Addr, time::SystemTime};

#[derive(Debug, sqlx::FromRow)]
pub struct DataModel {
    pub src_ip: Ipv4Addr, pub dst_ip: Ipv4Addr,
    pub src_port: u16, pub dst_port: u16,
    pub protocol: u8,
    pub total_bytes: u64,
    pub total_packet_count: u32,
    pub rate: f64,
    pub start_time: SystemTime,
    pub end_time: Option<SystemTime>,
    pub duration: u64
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
        duration: u64
    ) -> Self {
        return Self {
           src_ip, dst_ip,
           src_port, dst_port,
           protocol,
           total_bytes,
           total_packet_count,
           rate: DataModel::calculate_rate(total_bytes as f64, duration as f64),
           start_time, end_time,
           duration
        }
    }

    fn calculate_rate(size: f64, duration: f64) -> f64 {
        size / duration
    }
}