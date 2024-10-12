use std::{net::Ipv4Addr, time::SystemTime};

#[derive(Debug, sqlx::FromRow)]
pub struct DataModel {
    // Data for visualisation in frontend
    pub src_ip: Ipv4Addr, 
    pub dst_ip: Ipv4Addr,
    pub src_port: u16, 
    pub dst_port: u16,
    pub protocol: u8,
    
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

        start_time: SystemTime,
        last_update_time: SystemTime,
        duration: f32
    ) -> Self {
        return Self {
            src_ip, dst_ip,
            src_port, dst_port,
            protocol,

            start_time, last_update_time,
            duration
        }
    }
}