use std::{net::Ipv4Addr, time::SystemTime};

use serde::Serialize;

#[derive(Debug, sqlx::FromRow, Serialize)]
pub struct DataModel {
    // Data for visualisation in frontend
    pub src_ip: Ipv4Addr, 
    pub dst_ip: Ipv4Addr,
    pub src_port: u16, 
    pub dst_port: u16,
    pub protocol: u8,
    pub total_bytes: u64,
    pub total_packet_count: u32,

    // Model input
    pub sbytes: u64,
    pub smean: u64,
    pub dmean: u64,
    pub dbytes: u64,
    pub dload: f64,
    pub sload: f64,
    pub dpkts: u32,
    pub rate: f64,
    pub sttl: u8,
    pub dttl: u8,
    pub spkts: u32,
    pub checksum: u16,
    pub dscp: u8,
    pub ecn: u8,
    pub flags: u8,
    pub fragm_offset: u16,
    pub header_len: u8,
    
    // Time
    pub start_time: SystemTime,
    pub last_update_time: SystemTime,
    pub duration: f32
}

impl DataModel {
    pub fn new(
        src_ip: Ipv4Addr, 
        dst_ip: Ipv4Addr,
        src_port: u16, 
        dst_port: u16,
        protocol: u8,
        total_bytes: u64,
        total_packet_count: u32,
        source_packet_count: u32,
        destination_packet_count: u32,

        sbytes: u64,
        dbytes: u64,
        dttl: u8,
        sttl: u8,
        checksum: u16,
        dscp: u8,
        ecn: u8,
        flags: u8,
        fragm_offset: u16,
        header_len: u8,

        start_time: SystemTime,
        last_update_time: SystemTime,
        duration: f32
    ) -> Self {
        return Self {
            src_ip, dst_ip,
            src_port, dst_port,
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
            checksum,
            dscp,
            ecn,
            flags,
            fragm_offset,
            header_len,

            start_time, last_update_time,
            duration
        }
    }

    fn calculate_rate(size: f64, duration: f64) -> f64 {
        if duration == 0.0 { 0.0 } else {
            size / duration
        }
    }

    fn calculate_mean(size: u64, total_packet_count: u64) -> u64 {
        if total_packet_count == 0 { 0 } else {
            size / total_packet_count
        }
    }

    fn calculate_load(bytes: f64, duration: f64) -> f64 {
        if duration == 0.0 { 0.0 } else {
            bytes / duration
        }
    }
}