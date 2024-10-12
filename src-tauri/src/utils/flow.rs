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

    // Model input
    pub max_fwd_packet_length: u64,


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

        start_time: SystemTime, 
        last_update_time: SystemTime, 
    ) -> Self {
        Flow {
            src_ip, 
            dst_ip,
            src_port, 
            dst_port,
            protocol,

            max_fwd_packet_length: 0,

            start_time, 
            last_update_time, 
            finished: false,
        }
    }

    pub fn update(&mut self, size: u64, last_update_time: SystemTime, src_ip: Ipv4Addr, dst_ip: Ipv4Addr) {
        // Update flow based on direction
        if src_ip == self.src_ip && dst_ip == self.dst_ip {

            // Update max forward packet length
            if size as u64 > self.max_fwd_packet_length {
                self.max_fwd_packet_length = size as u64; 
            }
        } else {

        }

        self.last_update_time = last_update_time;
    }

    // NOTE: TO BE REMOVED LATER WHEN CAPTURED ON FRONTEND
    pub fn pretty_print(&self, prefix: &str) {
        println!(
            "{} ||| {}:{} -> {}:{} | Protocol: {} | Start Time: {} | Last Updated: {}",
            prefix,
            self.src_ip, self.src_port,
            self.dst_ip, self.dst_port,
            self.protocol,
            system_time_to_date_time(self.start_time), system_time_to_date_time(self.last_update_time)
        )
    }

    // NOTE: TO BE REMOVED LATER WHEN CAPTURED ON FRONTEND
    pub fn flow_termination_print(&self) {
        println!(
            "FLOW ||| {}:{} -> {}:{} TERMINATED | START: {} | LAST: {} | DURATION: {:?}",
            self.src_ip, self.src_port,
            self.dst_ip, self.dst_port,
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
