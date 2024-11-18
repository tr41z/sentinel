use std::sync::{Arc, Mutex, MutexGuard};
use std::time::{Duration, SystemTime};
use std::{collections::HashMap, net::Ipv4Addr};

use pnet::packet::ip::IpNextHeaderProtocol;

use sqlx::{Sqlite, Pool};

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
    checksum: u16,
    dscp: u8,
    ecn: u8,
    flags: u8,
    fragm_offset: u16,
    header_len: u8,

    flows_map: Arc<Mutex<HashMap<FlowKey, Flow>>>,
    db: &Pool<Sqlite>,
    local_ip: Ipv4Addr,
) {
    let mut flows_map: MutexGuard<'_, HashMap<FlowKey, Flow>> = flows_map.lock().unwrap();

    flows_map.retain(|_, flow: &mut Flow| !flow.finished);

    for (_, flow) in flows_map.iter_mut() {
        terminate_flows(flow, db).await;
    }

    let flow_key: FlowKey = FlowKey::new(src_ip, dst_ip, src_port, dst_port, protocol.0);

    if src_ip == local_ip {
        let reverse_flow_key = FlowKey::new(dst_ip, src_ip, dst_port, src_port, protocol.0);
        if let Some(flow) = flows_map.get_mut(&reverse_flow_key) {
            flow.update(size as u64, SystemTime::now(), src_ip, dst_ip, ttl);
            flow.dbytes += size as u64;
            flow.destination_packet_count += 1;
            flow.pretty_print("Reverse Flow Updated");
            return;
        } else {
            return;
        }
    }

    match flows_map.get_mut(&flow_key) {
        Some(flow) => {
            if !flow.finished {
                flow.update(size as u64, SystemTime::now(), src_ip, dst_ip, ttl);
                flow.packet_count += 1;
                flow.pretty_print("Forward Flow Updated");
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
                checksum,
                dscp,
                ecn,
                flags,
                fragm_offset,
                header_len,
                SystemTime::now(),
                SystemTime::now(),
            );

            new_flow.sbytes += size as u64;
            new_flow.source_packet_count += 1;
            new_flow.sttl = Some(ttl);

            flows_map.insert(flow_key, new_flow);
            new_flow.pretty_print("New Flow");
        }
    }
}

async fn terminate_flows(flow: &mut Flow, db: &Pool<Sqlite>) {
    let now: SystemTime = SystemTime::now();

    if let Ok(duration_since_last_update) = now.duration_since(flow.last_update_time) {
        if duration_since_last_update.as_secs_f64() >= 10.0 {
            flow.finished = true;
            flow.flow_termination_print();
            save_flow_to_db(flow, db, None).await;
            return;
        }
    }

    if let Ok(duration_since_start) = now.duration_since(flow.start_time) {
        if duration_since_start.as_secs_f32() >= 300.0 {
            flow.finished = true;
            flow.flow_termination_print();
            save_flow_to_db(flow, db, Some(300.0)).await;
        }
    }
}

async fn save_flow_to_db(flow: &mut Flow, db: &Pool<Sqlite>, forced_duration: Option<f32>) {
    let flow_duration_result = flow.last_update_time.duration_since(flow.start_time);
    let flow_duration_in_secs: f32 = forced_duration.unwrap_or_else(|| match flow_duration_result {
        Ok(flow_duration) => duration_to_secs(flow_duration),
        Err(_) => {
            eprintln!("Failed to calculate flow duration, using 0 as fallback");
            0.0
        }
    });

    let data_model: DataModel = database::model::DataModel::new(
        flow.src_ip, flow.dst_ip,
        flow.src_port, flow.dst_port, 
        flow.protocol, 
        flow.total_bytes, 
        flow.source_packet_count + flow.destination_packet_count, 
        flow.source_packet_count,
        flow.destination_packet_count,
        flow.sbytes,
        flow.dbytes,
        match flow.dttl { Some(dttl) => dttl, None => 0 },
        match flow.sttl { Some (sttl) => sttl, None => 0 },
        flow.checksum,
        flow.dscp,
        flow.ecn,
        flow.flags,
        flow.fragm_offset,
        flow.header_len,
        flow.start_time, flow.last_update_time,
        if flow_duration_in_secs <= 0.001 { 0.0 } else { flow_duration_in_secs }
    );

    match database::db::save_flow(db, data_model).await {
        Ok(saved_flow) => println!("Flow saved successfully: {:#?}", saved_flow),
        Err(e) => eprintln!("Failed to save flow: {:?}", e),
    };
}

fn duration_to_secs(duration: Duration) -> f32 {
    duration.as_secs_f32()
}
