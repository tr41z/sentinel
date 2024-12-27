use sqlx::{pool::PoolOptions, Error, Executor, Pool, Row, Sqlite, SqlitePool};

use std::{env, fs, net::Ipv4Addr, str::FromStr, time::SystemTime};

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
    let db_path: std::path::PathBuf = sentinel_dir.join("app_data.db");
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

async fn initialise_schema(pool: &SqlitePool) -> Result<(), Error> {
    // Get the current directory
    let current_dir: std::path::PathBuf = env::current_dir().map_err(Error::Io)?;
    // Create the relative path to the schema.sql file
    let schema_path: std::path::PathBuf = current_dir.join("src/database/migrations/schema.sql");

    let schema: String =
        fs::read_to_string(schema_path).map_err(|e: std::io::Error| Error::Io(e))?;
    pool.execute(schema.as_str()).await?;
    Ok(())
}

pub async fn save_flow(pool: &SqlitePool, flow: DataModel) -> Result<(), Error> {
    initialise_schema(pool).await?;

    let query = r#"
        INSERT INTO flows (
            src_ip, src_port, dst_ip, dst_port, protocol, 
            total_bytes, total_packet_count,
            sbytes, smean, dmean, dbytes, dload, sload, dpkts, rate, dttl, sttl, spkts,
            start_time, last_updated_time, dur
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"#;

    let result: Result<sqlx::sqlite::SqliteQueryResult, Error> = sqlx::query(query)
        .bind(flow.src_ip.to_string())
        .bind(flow.src_port)
        .bind(flow.dst_ip.to_string())
        .bind(flow.dst_port)
        .bind(flow.protocol)
        .bind(flow.total_bytes as i64)
        .bind(flow.total_packet_count)
        .bind(flow.sbytes as i64)
        .bind(flow.smean as i64)
        .bind(flow.dmean as i64)
        .bind(flow.dbytes as i64)
        .bind(flow.dload)
        .bind(flow.sload)
        .bind(flow.dpkts)
        .bind(flow.rate)
        .bind(flow.dttl)
        .bind(flow.sttl)
        .bind(flow.spkts)
        .bind(system_time_to_timestamp(flow.start_time))
        .bind(system_time_to_timestamp(flow.last_update_time))
        .bind(flow.duration)
        .execute(pool)
        .await;

    match result {
        Err(e) => {
            eprintln!("Error inserting flow: {:#?}!", flow);
            eprintln!("Error message: [{}].", e);
        }
        Ok(res) => {
            println!(
                "Flow inserted successfully! Rows affected: {}",
                res.rows_affected()
            );
        }
    }

    Ok(())
}

pub async fn get_all_flows(pool: &SqlitePool) -> Result<Vec<DataModel>, Error> {
    let query: &str = r#"
        SELECT 
<<<<<<< HEAD
            src_ip, dst_ip, protocol, 
            total_bytes, rate, avg_packet_size, total_packet_count, src_port_count, dst_port_count,
            start_time, last_updated_time, duration
=======
            src_ip, src_port, dst_ip, dst_port, protocol, 
            total_bytes, total_packet_count,
            sbytes, smean, dmean, dbytes, dload, sload, dpkts, rate, dttl, sttl, spkts,
            start_time, last_updated_time, dur
>>>>>>> parent of e2e037d (removing sniffer from `rust` due to `c++` implementation)
        FROM flows
    "#;

    let rows: Vec<sqlx::sqlite::SqliteRow> = sqlx::query(query).fetch_all(pool).await?;

    let flows: Vec<DataModel> = rows
        .into_iter()
        .map(|row: sqlx::sqlite::SqliteRow| DataModel {
            src_ip: Ipv4Addr::from_str(row.get::<String, _>("src_ip").as_str()).unwrap(),
            dst_ip: Ipv4Addr::from_str(row.get::<String, _>("dst_ip").as_str()).unwrap(),
            protocol: row.get("protocol"),
            total_bytes: row.get::<i64, _>("total_bytes") as u64,
<<<<<<< HEAD
            rate: row.get("rate"),
            avg_packet_size: row.get("avg_packet_size"),
            total_packet_count: row.get::<i64, _>("total_packet_count") as u64,
            src_port_count: row.get("src_port_count"),
            dst_port_count: row.get("dst_port_count"),
            start_time: timestamp_to_system_time(row.get("start_time")),
            last_update_time: timestamp_to_system_time(row.get("last_updated_time")),
            duration: row.get("duration"),
=======
            total_packet_count: row.get("total_packet_count"),
            sbytes: row.get::<i64, _>("sbytes") as u64,
            smean: row.get::<i64, _>("smean") as u64,
            dmean: row.get::<i64, _>("dmean") as u64,
            dbytes: row.get::<i64, _>("dbytes") as u64,
            dload: row.get("dload"),
            sload: row.get("sload"),
            dpkts: row.get("dpkts"),
            rate: row.get("rate"),
            dttl: row.get("dttl"),
            sttl: row.get("sttl"),
            spkts: row.get("spkts"),
            start_time: timestamp_to_system_time(row.get("start_time")),
            last_update_time: timestamp_to_system_time(row.get("last_updated_time")),
            duration: row.get("dur"),
>>>>>>> parent of e2e037d (removing sniffer from `rust` due to `c++` implementation)
        })
        .collect();

    Ok(flows)
}

// Helper function to convert SystemTime to Unix timestamp (i64)
pub fn system_time_to_timestamp(time: SystemTime) -> i64 {
    time.duration_since(SystemTime::UNIX_EPOCH)
        .map(|dur: std::time::Duration| dur.as_secs() as i64)
        .unwrap_or(0) // fallback to 0 in case of error
}

pub fn timestamp_to_system_time(timestamp: i64) -> SystemTime {
    SystemTime::UNIX_EPOCH + std::time::Duration::from_secs(timestamp as u64)
}
