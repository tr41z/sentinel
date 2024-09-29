use std::time::{Duration, SystemTime, SystemTimeError};

use std::{collections::HashMap, net::Ipv4Addr};

use pnet::packet::ip::IpNextHeaderProtocol;
use sqlx::{MySql, Pool};

use crate::utils::flow::{Flow, FlowKey};

use crate::database::{self};

// WARNING: EVERYTHING IN THAT FUNCTION IS LOOPED
pub async fn handle_packet_flow(
    src_ip: Ipv4Addr,
    dst_ip: Ipv4Addr,
    src_port: u16,
    dst_port: u16,
    protocol: IpNextHeaderProtocol,
    size: u16,

    flows_map: &mut HashMap<FlowKey, Flow>,
    db: &Pool<MySql>
) {
    // Cleanup terminated flows before handling new packets
    flows_map.retain(|_, flow| !flow.finished);

    let flow: Flow = Flow::new(
        src_ip,
        dst_ip,
        src_port,
        dst_port,
        protocol.0,
        size as u64,
        SystemTime::now(),
        SystemTime::now(),
        None,
    );

    let flow_key: FlowKey = FlowKey::new(src_ip, dst_ip, src_port, dst_port, protocol.0);

    match flows_map.get_mut(&flow_key) {
        Some(flow) => {
            // Terminate flows that were inactive for specified duration
            terminate_flows(flow);

            // Update flow only if it's not finished
            if !flow.finished {
                flow.update(size as u64, SystemTime::now());
                flow.pretty_print("Flow Updated");
            } else {
                let data_model = database::model::DataModel::new(
                     flow.src_ip, flow.dst_ip,
                     flow.src_port, flow.dst_port, 
                     flow.protocol, 
                     flow.total_bytes, 
                     flow.packet_count, 
                     flow.start_time, flow.end_time,
                     duration_to_str(flow.end_time.unwrap().duration_since(flow.start_time))
                );

                match database::db::save_flow(db, data_model).await {
                    Ok(flow) => println!("Flow saved successfully. {:#?}", flow),
                    Err(e) => eprintln!("Failed to save flow: {:?}", e),
                };
            }
        }
        None => {
            flows_map.insert(flow_key, flow);
            flow.pretty_print("New Flow")
        }
    };
}

// Terminate flows that were inactive for more than 5 seconds
/* 
FIXME: The mechanism doesn't work correctly, it's waiting for the the same flow update and then it checks if it was >= 5 seconds.
TODO: Should work asynchronously, indepented of flow updates. Should be thread spawned for each flow with some kind of timer. 
*/
fn terminate_flows(flow: &mut Flow) {
    let now: SystemTime = SystemTime::now();
    
    if let Ok(duration) = now.duration_since(flow.last_update_time) {
        if duration.as_secs() >= 5 {
            flow.finished = true;
            flow.end_time = Some(now);
            flow.flow_termination_print();
        }
    }
}

fn duration_to_str(duration: Result<Duration, SystemTimeError>) -> String {
    match duration {
        Ok(duration) => format!("{}", duration.as_secs()),
        Err(_) => String::from("Error in converting duration to string!")
    }
}