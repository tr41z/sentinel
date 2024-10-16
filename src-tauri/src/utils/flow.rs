use std:: {net::Ipv4Addr, time::SystemTime};

use chrono::{DateTime, Utc};

use serde::Serialize;

#[derive(Serialize, Clone, Debug, Copy)]
pub struct Flow {
    pub src_ip: Ipv4Addr, 
    pub dst_ip: Ipv4Addr,
    pub src_port: u16, 
    pub dst_port: u16,
    pub protocol: u8,
    pub total_bytes: u64,
    pub packet_count: u32,
    pub source_packet_count: u32,
    pub destination_packet_count: u32,
    pub sttl: Option<u8>, // source -> dest first assigned ttl
    pub dttl: Option<u8>, // dest -> source first assigned ttl

    // Model input
    pub sbytes: u64, // source -> dest load (bytes)
    pub dbytes: u64, // dest -> source load (bytes)

    // Time
    pub start_time: SystemTime, 
    pub last_update_time: SystemTime, 
    pub finished: bool,
}

impl Flow {
    pub fn new(
        src_ip: Ipv4Addr, 
        dst_ip: Ipv4Addr,
        src_port: u16, 
        dst_port: u16,
        protocol: u8,
        size: u64,

        start_time: SystemTime, 
        last_update_time: SystemTime, 
    ) -> Self {
        Flow {
            src_ip, 
            dst_ip,
            src_port, 
            dst_port,
            protocol,
            total_bytes: size,
            packet_count: 1,
            source_packet_count: 0,
            destination_packet_count: 0,
            sttl: None,
            dttl: None,

            sbytes: 0,
            dbytes: 0,

            start_time, 
            last_update_time, 
            finished: false,
        }
    }

    pub fn update(&mut self, size: u64, last_update_time: SystemTime, src_ip: Ipv4Addr, dst_ip: Ipv4Addr, ttl: u8) {
        self.packet_count += 1;
        self.total_bytes += size;

        // Update flow based on direction
        if src_ip == self.src_ip && dst_ip == self.dst_ip {
            self.sbytes += size as u64;
            self.source_packet_count += 1;
            
            if self.sttl.is_none() {
                self.sttl = Some(ttl);
            }
        } else {
            self.dbytes += size as u64;
            self.destination_packet_count += 1;
            
            if self.dttl.is_none() {
                self.dttl = Some(ttl);
            }
        }

        self.last_update_time = last_update_time;
    }

    // NOTE: TO BE REMOVED LATER WHEN CAPTURED ON FRONTEND
    pub fn pretty_print(&self, prefix: &str) {
        println!(
            "{} ||| {}:{} -> {}:{} | Size: {} bytes | STTL: {:?} | DTTL: {:?} | Packet Count: {} | Protocol: {} | Start Time: {} | Last Updated: {}",
            prefix,
            self.src_ip, self.src_port,
            self.dst_ip, self.dst_port,
            self.total_bytes,
            Some(self.sttl), Some(self.dttl),
            self.packet_count,
            self.protocol,
            system_time_to_date_time(self.start_time), system_time_to_date_time(self.last_update_time)
        )
    }

    // NOTE: TO BE REMOVED LATER WHEN CAPTURED ON FRONTEND
    pub fn flow_termination_print(&self) {
        println!(
            "FLOW ||| {}:{} -> {}:{} TERMINATED | SBYTES: {} | DBYTES: {} | SIZE: {} | START: {} | LAST: {} | DURATION: {:?}",
            self.src_ip, self.src_port,
            self.dst_ip, self.dst_port,

            self.sbytes,
            self.dbytes,
            self.total_bytes,
            system_time_to_date_time(self.start_time),
            system_time_to_date_time(self.last_update_time),
            (self.last_update_time.duration_since(self.start_time))
        )
    }
}

pub fn system_time_to_date_time(system_time: SystemTime) -> String {
    let datetime: DateTime<Utc> = system_time.into();
    datetime.format("%Y-%m-%d %H:%M:%S").to_string()
}

#[derive(serde::Serialize, Eq, Hash, PartialEq, Debug)]
pub struct FlowKey {
    src_ip: Ipv4Addr, 
    dst_ip: Ipv4Addr,
    src_port: u16,
    dst_port: u16,
    protocol: u8,
}

impl FlowKey {
    pub fn new(
        src_ip: Ipv4Addr, 
        dst_ip: Ipv4Addr,
        src_port: u16, 
        dst_port: u16,
        protocol: u8,
    ) -> Self {
        // Sort IPs and ports to handle bidirectional flow
        let (low_ip, high_ip) = if src_ip < dst_ip {
            (src_ip, dst_ip)
        } else {
            (dst_ip, src_ip)
        };
        let (low_port, high_port) = if src_port < dst_port {
            (src_port, dst_port)
        } else {
            (dst_port, src_port)
        };

        FlowKey {
            src_ip: low_ip,
            dst_ip: high_ip,
            src_port: low_port,
            dst_port: high_port,
            protocol,
        }
    }
}
