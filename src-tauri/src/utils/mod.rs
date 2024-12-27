pub mod flow {
    use std::{net::Ipv4Addr, time::SystemTime};

    use chrono::{DateTime, Utc};

    use serde::Serialize;

    use std::collections::HashSet;

    #[derive(Serialize, Clone, Debug)]
    pub struct Flow {
        pub src_ip: Ipv4Addr,
        pub dst_ip: Ipv4Addr,
        pub protocol: u8,
        pub src_ports: HashSet<u16>,
        pub dst_ports: HashSet<u16>,
        pub total_bytes: u64,
        pub packet_count: u16,
        pub start_time: SystemTime,
        pub last_update_time: SystemTime,
        pub finished: bool,
    }

    impl Flow {
        pub fn new(
            src_ip: Ipv4Addr,
            dst_ip: Ipv4Addr,
            protocol: u8,
            size: u64,
            start_time: SystemTime,
            last_update_time: SystemTime,
        ) -> Self {
            Flow {
                src_ip,
                dst_ip,
                protocol,
                src_ports: HashSet::new(),
                dst_ports: HashSet::new(),
                total_bytes: size,
                packet_count: 1,
                start_time,
                last_update_time,
                finished: false,
            }
        }

        pub fn update(
            &mut self,
            src_port: u16,
            dst_port: u16,
            size: u64,
            last_update_time: SystemTime,
        ) {
            self.packet_count = self
                .packet_count
                .checked_add(1)
                .expect("packet_count overflow");
            self.total_bytes = self
                .total_bytes
                .checked_add(size)
                .expect("total_bytes overflow");

            self.src_ports.insert(src_port);
            self.dst_ports.insert(dst_port);

            self.last_update_time = last_update_time;
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
        protocol: u8,
    }

    impl FlowKey {
        pub fn new(src_ip: Ipv4Addr, dst_ip: Ipv4Addr, protocol: u8) -> Self {
            FlowKey {
                src_ip,
                dst_ip,
                protocol,
            }
        }
    }
}
