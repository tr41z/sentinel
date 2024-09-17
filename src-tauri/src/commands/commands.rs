use std::thread;
use std::thread::JoinHandle;

use pnet::datalink;
use pnet::datalink::NetworkInterface;

use crate::services::capture::capture_packets;

// all interfaces
pub fn start_sniffer() {
    // FIXME: Apply threads for each interface (or many threads for one interface)
    let interfaces: Vec<NetworkInterface> = datalink::interfaces();
    let mut handles: Vec<JoinHandle<()>> = vec![];

    for interface in interfaces {
        let handle: JoinHandle<()> = thread::spawn(move || {
            capture_packets(interface);
        });
        handles.push(handle);
    }
}
