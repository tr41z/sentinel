use sqlx::{Error, Executor, MySql, MySqlPool, Pool};

use dotenv::dotenv;
use std::{env, fs};

use crate::utils::flow;

use super::model::DataModel;

pub async fn connect() -> Result<Pool<MySql>, Error> {
    dotenv().ok();

    let username: String = env::var("DB_USERNAME").expect("'DB_USERNAME' must be set!");
    let password: String = env::var("DB_PASSWORD").expect("'DB_PASSWORD' must be set!");
    let db_name: String = env::var("DB_NAME").expect("'DB_NAME' must be set!");
    let db_url: String = env::var("DB_URL").expect("'DB_URL' must be set!");

    let connection_string: String = format!("mysql://{}:{}@{}/{}", username, password, db_url, db_name);
    return MySqlPool::connect(&connection_string).await;
}

async fn initialise_schema(pool: &MySqlPool) -> Result<(), Error> {
    // Load schema.sql file
    let schema: String = fs::read_to_string("/Users/michael/Desktop/Coding/FYP/sentinel/src-tauri/src/database/migrations/schema.sql")
        .expect("Failed to read 'schema.sql'");

    pool.execute(schema.as_str()).await?;

    Ok(())
}

pub async fn save_flow(
    pool: &MySqlPool,
    flow: DataModel,
) -> Result<(), Error> {
    initialise_schema(pool).await?;

    let result = sqlx::query(
        "INSERT INTO flows (
            source_ip, destination_ip, source_port, destination_port, protocol, total_bytes, total_packet_count,
            start_time, end_time
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)")
        .bind(flow.src_ip)
        .bind(flow.dst_ip)
        .bind(flow.src_port)
        .bind(flow.dst_port)
        .bind(flow.protocol)
        .bind(flow.total_bytes)
        .bind(flow.total_packet_count)
        .bind(flow::system_time_to_date_time(flow.start_time))
        .bind(match flow.end_time {
            Some(time) => flow::system_time_to_date_time(time),
            None => String::from("NULL"),
        })
        .execute(pool)
        .await;

    match result {
        Err(e) => {
            println!("Error inserting flow: {:#?}!", flow);
            println!("Error message: [{}].\n", e.to_string());
        }
        Ok(res) => {
            println!("Flow has been inserted! Rows affected: {}", res.rows_affected());
        }
    }
    
    Ok(())
}