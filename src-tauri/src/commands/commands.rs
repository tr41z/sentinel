use std::thread;
use std::thread::JoinHandle;

use pnet::datalink;
use pnet::datalink::NetworkInterface;

use crate::database::db::{get_all_flows, connect};
use crate::database::model::DataModel;
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

// Retrieve all flows (async)
pub async fn get_flows() -> Result<Vec<DataModel>, sqlx::Error> {
    let pool = connect().await?;  // connect to the database
    let flows = get_all_flows(&pool).await?;  // fetch flows from the database
    Ok(flows)
}