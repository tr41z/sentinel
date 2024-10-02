use std::sync::{Arc, Mutex};
use std::time::{Duration, SystemTime, SystemTimeError};

use std::{collections::HashMap, net::Ipv4Addr};

use pnet::packet::ip::IpNextHeaderProtocol;
use sqlx::{MySql, Pool};

use crate::utils::flow::{Flow, FlowKey};

use crate::database::{self};
use crate::database::model::DataModel;

// WARNING: EVERYTHING IN THAT FUNCTION IS LOOPED
pub async fn handle_packet_flow(
    src_ip: Ipv4Addr,
    dst_ip: Ipv4Addr,
    src_port: u16,
    dst_port: u16,
    protocol: IpNextHeaderProtocol,
    size: u16,
    ttl: u8,

    flows_map: Arc<Mutex<HashMap<FlowKey, Flow>>>,
    db: &Pool<MySql>
) {
    // Lock the flows_map before accessing
    let mut flows_map = flows_map.lock().unwrap();

    // Cleanup terminated flows before handling new packets
    flows_map.retain(|_, flow: &mut Flow| !flow.finished);

    // Terminate inactive flows synchronously
    for (_, flow) in flows_map.iter_mut() {
        terminate_flows(flow, db).await; 
    }

    let flow_key: FlowKey = FlowKey::new(src_ip, dst_ip, src_port, dst_port, protocol.0);

    match flows_map.get_mut(&flow_key) {
        Some(flow) => {
            // Update flow only if it's not finished
            if !flow.finished {
                flow.update(size as u64, SystemTime::now(), src_ip, dst_ip, ttl);
                flow.pretty_print("Flow Updated");
            } 
        }
        None => {
            let mut new_flow: Flow = Flow::new(
                src_ip,
                dst_ip,
                src_port,
                dst_port,
                protocol.0,
                size as u64,
                SystemTime::now(),
                SystemTime::now(),
            );

            // Initialize the load based on direction
            if src_ip == new_flow.src_ip && dst_ip == new_flow.dst_ip {
                new_flow.sload += size as u64; // Update source to destination load
                new_flow.sttl = Some(ttl);
            } else {
                new_flow.dload += size as u64; // Update destination to source load
                new_flow.dttl = Some(ttl);
            }

            flows_map.insert(flow_key, new_flow);
            new_flow.pretty_print("New Flow");
        }
    };
}

// Terminate flows that were inactive for more than 60 seconds and save them to the database
async fn terminate_flows(flow: &mut Flow, db: &Pool<MySql>) {
    let now: SystemTime = SystemTime::now();
    
    if let Ok(duration) = now.duration_since(flow.last_update_time) {
        if duration.as_secs() >= 120 {
            flow.finished = true;
            flow.flow_termination_print();

            // Save the terminated flow to the database
            let data_model: DataModel = database::model::DataModel::new(
                flow.src_ip, flow.dst_ip,
                flow.src_port, flow.dst_port, 
                flow.protocol, 
                flow.total_bytes, 
                flow.packet_count, 
                flow.sload,
                flow.dload,
                flow.start_time, flow.last_update_time,
                duration_to_str(flow.last_update_time.duration_since(flow.start_time))
            );

            match database::db::save_flow(db, data_model).await {
                Ok(saved_flow) => println!("Flow saved successfully: {:#?}", saved_flow),
                Err(e) => eprintln!("Failed to save flow: {:?}", e),
            };
        }
    }
}

fn duration_to_str(duration: Result<Duration, SystemTimeError>) -> u64 {
    match duration {
        Ok(duration) => {
            if duration.as_secs() != 0 {
                duration.as_secs()
            } else {
                return 1;
            }
        },
        Err(e) => {
            eprintln!("Duration calculation failed: {:?}", e);
            1
        }
    }
}