use std::sync::{Arc, Mutex, MutexGuard};
use std::time::{Duration, SystemTime};
use std::{collections::HashMap, net::Ipv4Addr};

use log::{error, info};
use pnet::packet::ip::IpNextHeaderProtocol;

use sqlx::{Pool, Sqlite};

use crate::database::model::DataModel;
use crate::database::{self};
use crate::utils::flow::{Flow, FlowKey};

pub async fn handle_packet_flow(
    src_ip: Ipv4Addr,
    dst_ip: Ipv4Addr,
    src_port: u16,
    dst_port: u16,
    protocol: IpNextHeaderProtocol,
    size: u16,
    flows_map: Arc<Mutex<HashMap<FlowKey, Flow>>>,
    db: &Pool<Sqlite>,
    local_ip: Ipv4Addr,
) {
    let mut flows_map: MutexGuard<'_, HashMap<FlowKey, Flow>> = flows_map.lock().unwrap();

    flows_map.retain(|_, flow: &mut Flow| !flow.finished);

    for (_, flow) in flows_map.iter_mut() {
        terminate_flows(flow, db).await;
    }

    let flow_key: FlowKey = FlowKey::new(src_ip, dst_ip, protocol.0);

    if src_ip == local_ip {
        let reverse_flow_key = FlowKey::new(dst_ip, src_ip, protocol.0);
        if let Some(flow) = flows_map.get_mut(&reverse_flow_key) {
            flow.update(src_port, dst_port, size as u64, SystemTime::now());
            return;
        } else {
            return;
        }
    }

    match flows_map.get_mut(&flow_key) {
        Some(flow) => {
            if !flow.finished {
                flow.update(src_port, dst_port, size as u64, SystemTime::now());
            }
        }
        None => {
            let mut new_flow: Flow = Flow::new(
                src_ip,
                dst_ip,
                protocol.0,
                size as u64,
                SystemTime::now(),
                SystemTime::now(),
            );

            new_flow.src_ports.insert(src_port);
            new_flow.dst_ports.insert(dst_port);

            flows_map.insert(flow_key, new_flow);
        }
    }
}

async fn terminate_flows(flow: &mut Flow, db: &Pool<Sqlite>) {
    let now: SystemTime = SystemTime::now();

    if let Ok(duration_since_last_update) = now.duration_since(flow.last_update_time) {
        if duration_since_last_update.as_secs_f64() >= 10.0 {
            flow.finished = true;
            save_flow_to_db(flow, db, None).await;
            return;
        }
    }

    if let Ok(duration_since_start) = now.duration_since(flow.start_time) {
        if duration_since_start.as_secs_f32() >= 30.0 {
            flow.finished = true;
            save_flow_to_db(flow, db, Some(300.0)).await;
        }
    }
}

async fn save_flow_to_db(flow: &mut Flow, db: &Pool<Sqlite>, forced_duration: Option<f32>) {
    let flow_duration_result = flow.last_update_time.duration_since(flow.start_time);
    let flow_duration_in_secs: f32 =
        forced_duration.unwrap_or_else(|| match flow_duration_result {
            Ok(flow_duration) => duration_to_secs(flow_duration),
            Err(_) => {
                eprintln!("Failed to calculate flow duration, using 0 as fallback");
                0.0
            }
        });

    let data_model: DataModel = database::model::DataModel::new(
        flow.src_ip,
        flow.dst_ip,
        flow.src_ports.len() as u16,
        flow.dst_ports.len() as u16,
        flow.protocol,
        flow.total_bytes,
        flow.packet_count.into(),
        flow.start_time,
        flow.last_update_time,
        if flow_duration_in_secs <= 0.001 {
            0.0
        } else {
            flow_duration_in_secs
        },
    );

    match database::db::save_flow(db, data_model).await {
        Ok(saved_flow) => {
            info!("Flow saved successfully: {:#?}", saved_flow);
        }
        Err(e) => {
            error!("Failed to save flow: {:?}", e);
        }
    };
}

fn duration_to_secs(duration: Duration) -> f32 {
    duration.as_secs_f32()
}
