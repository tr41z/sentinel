use std::{net::Ipv4Addr, time::SystemTime};

use serde::Serialize;

#[derive(Debug, sqlx::FromRow, Serialize)]
pub struct DataModel {
    pub src_ip: Ipv4Addr,
    pub dst_ip: Ipv4Addr,
    pub protocol: u8,
    pub total_bytes: u64,
<<<<<<< HEAD
    pub rate: f64,
    pub avg_packet_size: f64,
    pub total_packet_count: u64,
    pub src_port_count: u64,
    pub dst_port_count: u64,
    pub start_time: SystemTime,
    pub last_update_time: SystemTime,
    pub duration: u64,
=======
    pub total_packet_count: u16,
    pub sbytes: u64,
    pub smean: u64,
    pub dmean: u64,
    pub dbytes: u64,
    pub dload: f64,
    pub sload: f64,
    pub dpkts: u16,
    pub rate: f64,
    pub sttl: u8,
    pub dttl: u8,
    pub spkts: u16,
    pub start_time: SystemTime,
    pub last_update_time: SystemTime,
    pub duration: f32,
}

impl DataModel {
    pub fn new(
        src_ip: Ipv4Addr,
        dst_ip: Ipv4Addr,
        src_port: u16,
        dst_port: u16,
        protocol: u8,
        total_bytes: u64,
        total_packet_count: u16,
        source_packet_count: u16,
        destination_packet_count: u16,
        sbytes: u64,
        dbytes: u64,
        dttl: u8,
        sttl: u8,
        start_time: SystemTime,
        last_update_time: SystemTime,
        duration: f32,
    ) -> Self {
        Self {
            src_ip,
            dst_ip,
            src_port,
            dst_port,
            protocol,
            total_bytes,
            total_packet_count,
            sbytes,
            smean: DataModel::calculate_mean(sbytes, source_packet_count as u64),
            dmean: DataModel::calculate_mean(dbytes, destination_packet_count as u64),
            dbytes,
            dload: DataModel::calculate_rate(dbytes as f64, duration as f64),
            sload: DataModel::calculate_load(sbytes as f64, duration as f64),
            dpkts: destination_packet_count,
            rate: DataModel::calculate_rate(total_bytes as f64, duration as f64),
            dttl,
            sttl,
            spkts: source_packet_count,
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
>>>>>>> parent of e2e037d (removing sniffer from `rust` due to `c++` implementation)
}
