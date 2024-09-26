use std:: {net::Ipv4Addr, time::SystemTime};

use chrono::{DateTime, Utc};
use serde::Serialize;

#[derive(Serialize, Clone, Debug, Copy)]
pub struct Flow {
    src_ip: Ipv4Addr,
    dst_ip: Ipv4Addr,
    src_port: u16,
    dst_port: u16,
    protocol: u8,
    pub total_bytes: u64,
    pub packet_count: u32,
    start_time: SystemTime,
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

    // NOTE: TO BE REMOVED LATER
    pub fn pretty_print(&self, prefix: &str) {
        println!(
            "{} ||| {}:{} -> {}:{} | Size: {} bytes | Packet Count: {} | Protocol: {} | Start Time: {} | Last Updated: {}",
            prefix,
            self.src_ip, self.src_port,
            self.dst_ip, self.dst_port,
            self.total_bytes,
            self.packet_count,
            self.protocol,
            system_time_to_date_time(self.start_time), system_time_to_date_time(self.last_update_time)
        )
    }

    // NOTE: TO BE REMOVED LATER
    pub fn flow_termination_print(&self) {
        println!(
            "FLOW ||| {}:{} -> {}:{} TERMINATED | Start Time: {} | End Time: {} | DURATION: {:?}",
            self.src_ip, self.src_port,
            self.dst_ip, self.dst_port,
            system_time_to_date_time(self.start_time), system_time_to_date_time(self.end_time.unwrap()),
            self.end_time.unwrap().duration_since(self.start_time)
        )
    }
}

fn system_time_to_date_time(time: SystemTime) -> String {
    let new_time: DateTime<Utc> = time.into();
    format!("{}", new_time.format("%d/%m/%Y %H:%M:%S")).to_string()
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
