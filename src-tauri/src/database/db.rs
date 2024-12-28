use super::model::DataModel;
use directories::ProjectDirs;
use log::{error, info};
use sqlx::{pool::PoolOptions, Error, Executor, Pool, Row, Sqlite, SqlitePool};
use std::{fs, net::Ipv4Addr, str::FromStr, time::SystemTime};

pub async fn connect() -> Result<Pool<Sqlite>, Error> {
    let connection_string = build_connection_string()?;

    // Log the connection string
    info!("Connecting to database at: {}", connection_string);

    let pool = PoolOptions::new()
        .max_connections(20)
        .connect(&connection_string)
        .await?;

    Ok(pool)
}

fn ensure_sentinel_dir() -> Result<std::path::PathBuf, std::io::Error> {
    let project_dirs = ProjectDirs::from("com", "sentinel", "app").ok_or_else(|| {
        std::io::Error::new(
            std::io::ErrorKind::NotFound,
            "Unable to find project directories",
        )
    })?;

    let sentinel_dir = project_dirs.data_dir().to_path_buf();

    if !sentinel_dir.exists() {
        fs::create_dir_all(&sentinel_dir)?;
        println!("Created directory: {}", sentinel_dir.display());
    }

    Ok(sentinel_dir)
}

fn build_connection_string() -> Result<String, sqlx::Error> {
    let sentinel_dir = ensure_sentinel_dir().map_err(sqlx::Error::Io)?;

    let db_path = sentinel_dir.join("app_data.db");
    let db_path_str = db_path.to_str().ok_or_else(|| {
        sqlx::Error::Configuration(Box::new(std::io::Error::new(
            std::io::ErrorKind::InvalidInput,
            "Invalid database path",
        )))
    })?;

    if !db_path.exists() {
        fs::File::create(&db_path).map_err(sqlx::Error::Io)?;
        println!("Created database file at: {}", db_path_str);
    }

    Ok(format!("sqlite://{}?mode=rwc&cache=shared", db_path_str))
}

fn get_schema() -> &'static str {
    include_str!("migrations/schema.sql")
}

async fn initialise_schema(pool: &SqlitePool) -> Result<(), Error> {
    let schema = get_schema();
    pool.execute(schema).await?;
    Ok(())
}

pub async fn save_flow(pool: &SqlitePool, flow: DataModel) -> Result<(), Error> {
    initialise_schema(pool).await?;

    let query = r#"
        INSERT INTO flows (
            src_ip, src_port_count, dst_ip, dst_port_count, protocol, 
            total_bytes, total_packet_count,
            start_time, last_updated_time, dur
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"#;

    let result: Result<sqlx::sqlite::SqliteQueryResult, Error> = sqlx::query(query)
        .bind(flow.src_ip.to_string())
        .bind(flow.src_port_count)
        .bind(flow.dst_ip.to_string())
        .bind(flow.dst_port_count)
        .bind(flow.protocol)
        .bind(flow.total_bytes as i64)
        .bind(flow.total_packet_count as i64)
        .bind(system_time_to_timestamp(flow.start_time))
        .bind(system_time_to_timestamp(flow.last_update_time))
        .bind(flow.duration)
        .execute(pool)
        .await;

    match result {
        Err(e) => {
            error!("Error inserting flow: {:#?}!", flow);
            error!("Error message: [{}].", e);
        }
        Ok(res) => {
            info!(
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
            src_ip, src_port_count, dst_ip, dst_port_count, protocol, 
            total_bytes, total_packet_count,
            start_time, last_updated_time, dur
        FROM flows
    "#;

    let rows: Vec<sqlx::sqlite::SqliteRow> = sqlx::query(query).fetch_all(pool).await?;

    let flows: Vec<DataModel> = rows
        .into_iter()
        .map(|row: sqlx::sqlite::SqliteRow| DataModel {
            src_ip: Ipv4Addr::from_str(row.get::<String, _>("src_ip").as_str()).unwrap(),
            src_port_count: row.get("src_port_count"),
            dst_ip: Ipv4Addr::from_str(row.get::<String, _>("dst_ip").as_str()).unwrap(),
            dst_port_count: row.get("dst_port_count"),
            protocol: row.get("protocol"),
            total_bytes: row.get::<i64, _>("total_bytes") as u64,
            total_packet_count: row.get("total_packet_count"),
            start_time: timestamp_to_system_time(row.get("start_time")),
            last_update_time: timestamp_to_system_time(row.get("last_updated_time")),
            duration: row.get("dur"),
        })
        .collect();

    Ok(flows)
}

pub fn system_time_to_timestamp(time: SystemTime) -> i64 {
    time.duration_since(SystemTime::UNIX_EPOCH)
        .map(|dur: std::time::Duration| dur.as_secs() as i64)
        .unwrap_or(0)
}

pub fn timestamp_to_system_time(timestamp: i64) -> SystemTime {
    SystemTime::UNIX_EPOCH + std::time::Duration::from_secs(timestamp as u64)
}
