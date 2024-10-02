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

    // Model input
    pub sload: u64, // source -> dest load (bytes)
    pub dload: u64, // dest -> source load (bytes)

    // Time
    pub start_time: SystemTime, 
    pub last_update_time: SystemTime, 
    pub end_time: Option<SystemTime>, 
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
        end_time: Option<SystemTime>,
    ) -> Self {
        Flow {
            src_ip, 
            dst_ip,
            src_port, 
            dst_port,
            protocol,
            total_bytes: size,
            packet_count: 1,

            sload: 0,
            dload: 0,

            start_time, 
            last_update_time, 
            end_time,
            finished: false,
        }
    }

    pub fn update(&mut self, size: u64, last_update_time: SystemTime) {
        self.packet_count += 1;
        self.total_bytes += size;

        self.last_update_time = last_update_time;
    }

    // NOTE: TO BE REMOVED LATER WHEN CAPTURED ON FRONTEND
    pub fn pretty_print(&self, prefix: &str) {
        println!(
            "{} ||| {}:{} -> {}:{} | Size: {} bytes | Sload: {} bytes | Dload: {} bytes | Packet Count: {} | Protocol: {} | Start Time: {} | Last Updated: {}",
            prefix,
            self.src_ip, self.src_port,
            self.dst_ip, self.dst_port,
            self.total_bytes,
            self.sload, self.dload,
            self.packet_count,
            self.protocol,
            system_time_to_date_time(self.start_time), system_time_to_date_time(self.last_update_time)
        )
    }

    // NOTE: TO BE REMOVED LATER WHEN CAPTURED ON FRONTEND
    pub fn flow_termination_print(&self) {
        println!(
            "FLOW ||| {}:{} -> {}:{} TERMINATED | Start Time: {} | Last Time Updated: {} | End Time: {} | DURATION: {:?}",
            self.src_ip, self.src_port,
            self.dst_ip, self.dst_port,

            system_time_to_date_time(self.start_time), 
            system_time_to_date_time(self.last_update_time), 
            system_time_to_date_time(self.end_time.unwrap()),
            self.end_time.unwrap().duration_since(self.start_time)
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
        FlowKey {
            src_ip, 
            dst_ip,
            src_port, 
            dst_port,
            protocol,
        }
    }
}
