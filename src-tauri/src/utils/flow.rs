use std::net::Ipv4Addr;

#[derive(serde::Serialize, Clone, Debug, Copy)]
pub struct Flow {
    src_ip: Ipv4Addr,
    dst_ip: Ipv4Addr,
    src_port: u16,
    dst_port: u16,
    protocol: u8,
    total_bytes: u64,
    packet_count: u32,
}

impl Flow {
    pub fn new(src_ip: Ipv4Addr, dst_ip: Ipv4Addr, src_port: u16, dst_port: u16, protocol: u8, size: u64) -> Self {
        Flow {
            src_ip, dst_ip,
            src_port, dst_port,
            protocol,
            total_bytes: size,
            packet_count: 1,
        }
    }
    pub fn update(&mut self, size: u64) {
        self.packet_count += 1;
        self.total_bytes += size;
    }

    pub fn pretty_print(&self, prefix: &str) {
        println!(
            "{} ||| {}:{} -> {}:{} | Size: {} bytes | Packet Count: {} | Protocol: {}",
            prefix,
            self.src_ip, self.dst_port,
            self.dst_ip, self.dst_port,
            self.total_bytes,
            self.packet_count,
            self.protocol
        )
    }
}

#[derive(serde::Serialize, Eq, Hash, PartialEq, Debug)]
pub struct FlowKey {
    src_ip: Ipv4Addr,
    dst_ip: Ipv4Addr,
    src_port: u16,
    dst_port: u16,
    protocol: u8
}

impl FlowKey {
    pub fn new(src_ip: Ipv4Addr, dst_ip: Ipv4Addr, src_port: u16, dst_port: u16, protocol: u8) -> Self {
        FlowKey {
            src_ip, dst_ip,
            src_port, dst_port,
            protocol
        }
    }
}