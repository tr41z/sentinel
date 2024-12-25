use sqlx::{pool::PoolOptions, Error, Pool, Row, Sqlite, SqlitePool};

use std::{fs, net::Ipv4Addr, str::FromStr, time::SystemTime};

use super::model::DataModel;

pub async fn connect() -> Result<Pool<Sqlite>, Error> {
    let connection_string = build_connection_string()?;

    // Log the connection string
    println!("Connecting to database at: {}", connection_string);

    let pool = PoolOptions::new()
        .max_connections(20)
        .connect(&connection_string)
        .await?;

    Ok(pool)
}

fn build_connection_string() -> Result<String, sqlx::Error> {
    // Get the user's home directory
    let home_dir: std::path::PathBuf = home::home_dir().ok_or_else(|| {
        sqlx::Error::Configuration(Box::new(std::io::Error::new(
            std::io::ErrorKind::Other,
            "Unable to determine home directory",
        )))
    })?;

    // Create the hidden .sentinel directory in the home directory
    let sentinel_dir: std::path::PathBuf = home_dir.join(".sentinel");
    fs::create_dir_all(&sentinel_dir).map_err(sqlx::Error::Io)?;

    // Create the full path to the SQLite database file in the .sentinel directory
    let db_path: std::path::PathBuf = sentinel_dir.join("sentinel.db");
    let db_path_str: &str = db_path.to_str().ok_or_else(|| {
        sqlx::Error::Configuration(Box::new(std::io::Error::new(
            std::io::ErrorKind::Other,
            "Invalid database path",
        )))
    })?;

    // Ensure the database file exists
    if !db_path.exists() {
        fs::File::create(&db_path).map_err(|e: std::io::Error| sqlx::Error::Io(e))?;
        println!("Created database file at: {}", db_path_str);
    }

    // SQLite connection string with WAL mode enabled
    Ok(format!("sqlite://{}?mode=rwc&cache=shared", db_path_str))
}

pub async fn get_all_flows(pool: &SqlitePool) -> Result<Vec<DataModel>, Error> {
    let query: &str = r#"
        SELECT 
            src_ip, src_port, dst_ip, dst_port, protocol, 
            total_bytes, total_packet_count,
            start_time, last_updated_time
        FROM flows
    "#;

    let rows: Vec<sqlx::sqlite::SqliteRow> = sqlx::query(query).fetch_all(pool).await?;

    let flows: Vec<DataModel> = rows
        .into_iter()
        .map(|row: sqlx::sqlite::SqliteRow| DataModel {
            src_ip: Ipv4Addr::from_str(row.get::<String, _>("src_ip").as_str()).unwrap(),
            src_port: row.get("src_port"),
            dst_ip: Ipv4Addr::from_str(row.get::<String, _>("dst_ip").as_str()).unwrap(),
            dst_port: row.get("dst_port"),
            protocol: row.get("protocol"),
            total_bytes: row.get::<i64, _>("total_bytes") as u64,
            total_packet_count: row.get("total_packet_count"),
            start_time: timestamp_to_system_time(row.get("start_time")),
            last_update_time: timestamp_to_system_time(row.get("last_updated_time")),
        })
        .collect();

    Ok(flows)
}

pub fn timestamp_to_system_time(timestamp: i64) -> SystemTime {
    SystemTime::UNIX_EPOCH + std::time::Duration::from_secs(timestamp as u64)
}
