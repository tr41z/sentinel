use std::thread;

use pnet::datalink;

use crate::services::capture::capture_packets;

// all interfaces
pub fn start_sniffer() {
    // FIXME: Apply threads for each interface (or many threads for one interface)
    let interfaces = datalink::interfaces();
    let mut handles = vec![];

    for interface in interfaces {
        let handle = thread::spawn(move || {
            capture_packets(interface);
        });
        handles.push(handle);
    }
}
