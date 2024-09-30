use std::thread;
use std::thread::JoinHandle;

use pnet::datalink;
use pnet::datalink::NetworkInterface;

use crate::services::capture::capture_packets;

// All interfaces
pub fn start_sniffer() {
    let interfaces: Vec<NetworkInterface> = datalink::interfaces();
    let mut handles: Vec<JoinHandle<()>> = vec![];

    for interface in interfaces {
        let handle: JoinHandle<()> = thread::spawn(move || {
            capture_packets(interface);
        });
        handles.push(handle);
    }
}
