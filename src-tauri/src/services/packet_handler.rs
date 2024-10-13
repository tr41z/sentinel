use std::sync::{Arc, Mutex, MutexGuard};
use std::time::{Duration, SystemTime};

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
    let mut flows_map: MutexGuard<'_, HashMap<FlowKey, Flow>> = flows_map.lock().unwrap();

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
                new_flow.sbytes += size as u64; // Update source to destination load
                new_flow.source_packet_count += 1;
                new_flow.sttl = Some(ttl);
            } else {
                new_flow.dbytes += size as u64; // Update destination to source load
                new_flow.destination_packet_count += 1;
                new_flow.dttl = Some(ttl);
            }

            flows_map.insert(flow_key, new_flow);
            new_flow.pretty_print("New Flow");
        }
    };
}

// Terminate flows that were inactive or have been active for 300 seconds
async fn terminate_flows(flow: &mut Flow, db: &Pool<MySql>) {
    let now: SystemTime = SystemTime::now();

    // Check for inactive flows (inactive for 120 seconds)
    if let Ok(duration_since_last_update) = now.duration_since(flow.last_update_time) {
        if duration_since_last_update.as_secs_f64() >= 5.0 {
            flow.finished = true;
            flow.flow_termination_print();

            // Save terminated flow to the database (for inactive flow)
            save_flow_to_db(flow, db, None).await;
            return;
        }
    }

    // Check if flow has been active for 300 seconds or more
    if let Ok(duration_since_start) = now.duration_since(flow.start_time) {
        if duration_since_start.as_secs_f32() >= 300.0 {
            flow.finished = true;
            flow.flow_termination_print();

            // Save terminated flow to the database (for active flow > 300 seconds)
            save_flow_to_db(flow, db, Some(300.0)).await;
        }
    }
}


// Helper function to save terminated flow to the database
async fn save_flow_to_db(flow: &mut Flow, db: &Pool<MySql>, forced_duration: Option<f32>) {
    // Calculate duration between start_time and last_update_time
    let flow_duration_result = flow.last_update_time.duration_since(flow.start_time);

    // Convert the duration from Result to f32 seconds, or use the forced duration if provided
    let flow_duration_in_secs: f32 = forced_duration.unwrap_or_else(|| match flow_duration_result {
        Ok(flow_duration) => duration_to_secs(flow_duration),
        Err(_) => {
            eprintln!("Failed to calculate flow duration, using 0 as fallback");
            0.0 // Fallback value in case of an error
        }
    });

    // Prepare DataModel to save to the database
    let data_model: DataModel = database::model::DataModel::new(
        flow.src_ip, flow.dst_ip,
        flow.src_port, flow.dst_port, 
        flow.protocol, 
        flow.total_bytes, 
        flow.packet_count, 
        flow.source_packet_count,
        flow.destination_packet_count,
        flow.sbytes,
        flow.dbytes,
        flow.start_time, flow.last_update_time,
        flow_duration_in_secs
    );

    // Save the flow to the database
    match database::db::save_flow(db, data_model).await {
        Ok(saved_flow) => println!("Flow saved successfully: {:#?}", saved_flow),
        Err(e) => eprintln!("Failed to save flow: {:?}", e),
    };
}

fn duration_to_secs(duration: Duration) -> f32 {
    duration.as_secs_f32()
}