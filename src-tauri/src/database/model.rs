use std::{net::Ipv4Addr, time::SystemTime};

use serde::Serialize;

#[derive(Debug, sqlx::FromRow, Serialize)]
pub struct DataModel {
    pub src_ip: Ipv4Addr,
    pub dst_ip: Ipv4Addr,
    pub start_time: SystemTime,
    pub last_update_time: SystemTime,
    pub duration: f32,
}

impl DataModel {
    pub fn new(
        src_ip: Ipv4Addr,
        dst_ip: Ipv4Addr,
        src_port_count: u16,
        dst_port_count: u16,
        protocol: u8,
        total_bytes: u64,
        total_packet_count: u64,
        start_time: SystemTime,
        last_update_time: SystemTime,
        duration: f32,
    ) -> Self {
        Self {
            src_ip,
            dst_ip,
            src_port_count,
            dst_port_count,
            protocol,
            total_bytes,
            total_packet_count,
            start_time,
            last_update_time,
            duration,
        }
    }

    pub fn calculate_rate(size: f64, duration: f64) -> f64 {
        if duration <= 0.0 || size <= 0.0 {
            0.0
        } else {
            size / duration
        }
    }

    pub fn calculate_mean(size: u64, total_packet_count: u64) -> u64 {
        if total_packet_count == 0 {
            0
        } else {
            size / total_packet_count
        }
    }

    pub fn calculate_load(bytes: f64, duration: f64) -> f64 {
        if duration <= 0.0 || bytes <= 0.0 {
            0.0
        } else {
            bytes / duration
        }
    }
}
