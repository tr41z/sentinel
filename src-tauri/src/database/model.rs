use std::{net::Ipv4Addr, time::SystemTime};

#[derive(Debug, sqlx::FromRow)]
pub struct DataModel {
    // Data for visualisation in frontend
    pub src_ip: Ipv4Addr, 
    pub dst_ip: Ipv4Addr,
    pub src_port: u16, 
    pub dst_port: u16,
    pub protocol: u8,
    pub total_bytes: u64,
    pub total_packet_count: u32,
    pub dttl: u8,

    // Model input
    pub rate: f64,
    pub sbytes: u64,
    pub dbytes: u64,
    pub sttl: u8,
    pub smean: u64,
    pub sload: f64,
    pub dload: f64,
    // NOTE: ADD ct_state_ttl, ct_dst_src_ltm, ct_srv_dst
    
    // Time
    pub start_time: SystemTime,
    pub last_update_time: SystemTime,
    pub duration: u64
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

        sbytes: u64,
        dbytes: u64,

        sttl: u8,
        dttl: u8,

        start_time: SystemTime,
        last_update_time: SystemTime,
        duration: u64
    ) -> Self {
        return Self {
            src_ip, dst_ip,
            src_port, dst_port,
            protocol,
            total_bytes,
            total_packet_count,

            rate: DataModel::calculate_rate(total_bytes as f64, duration as f64),
            sbytes,
            dbytes,
            sttl,
            dttl,
            sload: DataModel::calculate_load(sbytes as f64, duration as f64),
            dload: DataModel::calculate_rate(dbytes as f64, duration as f64),
            smean: DataModel::calculate_smean(sbytes, source_packet_count as u64),

            start_time, last_update_time,
            duration
        }
    }

    fn calculate_rate(size: f64, duration: f64) -> f64 {
        if duration == 0.0 { 0.0 } else {
            size / duration
        }
    }

    fn calculate_smean(size: u64, total_packet_count: u64) -> u64 {
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