use tauri::AppHandle;

use pnet::datalink::NetworkInterface;
use pnet::datalink::interfaces;

use crate::services::capture::capture_packets;

// en0 interface
pub fn start_sniffer(app_handle: AppHandle) {
    // FIXME: Apply threads for each interface (or many threads for one interface)
    let interfaces: Vec<NetworkInterface> = interfaces();

    for interface in interfaces {
        capture_packets(interface, app_handle.clone());
    }
}
