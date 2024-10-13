use sqlx::{Error, Executor, MySql, MySqlPool, Pool};
use dotenv::dotenv;
use std::{env, fs};
use crate::utils::flow;

use super::model::DataModel;

pub async fn connect() -> Result<Pool<MySql>, Error> {
    dotenv().ok();
    let connection_string = build_connection_string()?;
    MySqlPool::connect(&connection_string).await
}

fn build_connection_string() -> Result<String, Error> {
    let username = env::var("DB_USERNAME").map_err(|e| {
        sqlx::Error::Configuration(Box::new(e))
    })?;

    let password = env::var("DB_PASSWORD").map_err(|e| {
        sqlx::Error::Configuration(Box::new(e))
    })?;

    let db_name = env::var("DB_NAME").map_err(|e| {
        sqlx::Error::Configuration(Box::new(e))
    })?;

    let db_url = env::var("DB_URL").map_err(|e| {
        sqlx::Error::Configuration(Box::new(e))
    })?;

    Ok(format!("mysql://{}:{}@{}/{}", username, password, db_url, db_name))
}

async fn initialise_schema(pool: &MySqlPool) -> Result<(), Error> {
    let schema = fs::read_to_string("/Users/michael/Desktop/Coding/FYP/sentinel/src-tauri/src/database/migrations/schema.sql")
        .map_err(|e| Error::Io(e))?;
    pool.execute(schema.as_str()).await?;
    Ok(())
}

pub async fn save_flow(pool: &MySqlPool, flow: DataModel) -> Result<(), Error> {
    initialise_schema(pool).await?;

    // NOTE: ADD `sloss` and `dloss`
    let query: &str = "INSERT INTO flows (
            src_ip, src_port, dst_ip, dst_port, protocol, 
            total_bytes, total_packet_count,
            sbytes, smean, dmean, dbytes, dload, sload, dpkts, rate, dttl, spkts,
            start_time, last_updated_time, dur
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    let result = sqlx::query(query)
        .bind(flow.src_ip)
        .bind(flow.src_port)
        .bind(flow.dst_ip)
        .bind(flow.dst_port)
        .bind(flow.protocol)
        .bind(flow.total_bytes)
        .bind(flow.total_packet_count)

        .bind(flow.sbytes)
        .bind(flow.smean)
        .bind(flow.dmean)
        .bind(flow.dbytes)
        .bind(flow.dload)
        .bind(flow.sload)
        .bind(flow.dpkts)
        .bind(flow.rate)
        .bind(flow.dttl)
        .bind(flow.spkts)

        .bind(flow::system_time_to_date_time(flow.start_time))
        .bind(flow::system_time_to_date_time(flow.last_update_time))
        .bind(flow.duration)
        .execute(pool)
        .await;

    match result {
        Err(e) => {
            eprintln!("Error inserting flow: {:#?}!", flow);
            eprintln!("Error message: [{}].", e.to_string());
        }
        Ok(res) => {
            println!("Flow inserted successfully! Rows affected: {}", res.rows_affected());
        }
    }
    
    Ok(())
}
