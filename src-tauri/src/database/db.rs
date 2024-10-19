use sqlx::{Error, Executor, Pool, Sqlite, SqlitePool};

use std::{env, fs, time::SystemTime};

use super::model::DataModel;

pub async fn connect() -> Result<Pool<Sqlite>, Error> {
    let connection_string = build_connection_string()?;
    
    // Log the connection string
    println!("Connecting to database at: {}", connection_string);
    
    SqlitePool::connect(&connection_string).await
}

fn build_connection_string() -> Result<String, Error> {
    // Get the user's home directory
    let home_dir = home::home_dir().ok_or_else(|| {
        sqlx::Error::Configuration(Box::new(std::io::Error::new(
            std::io::ErrorKind::Other,
            "Unable to determine home directory"
        )))
    })?;

    // Create the hidden .sentinel directory in the home directory
    let sentinel_dir = home_dir.join(".sentinel");
    fs::create_dir_all(&sentinel_dir).map_err(|e| {
        sqlx::Error::Io(e)
    })?;

    // Create the full path to the SQLite database file in the .sentinel directory
    let db_path = sentinel_dir.join("app_data.db");
    let db_path_str = db_path.to_str().ok_or_else(|| {
        sqlx::Error::Configuration(Box::new(std::io::Error::new(
            std::io::ErrorKind::Other,
            "Invalid database path"
        )))
    })?;

    // Ensure the database file exists
    if !db_path.exists() {
        fs::File::create(&db_path).map_err(|e| {
            sqlx::Error::Io(e)
        })?;
        println!("Created database file at: {}", db_path_str);
    }

    // SQLite connection string
    Ok(format!("sqlite://{}", db_path_str))
}

async fn initialise_schema(pool: &SqlitePool) -> Result<(), Error> {
    // Get the current directory
    let current_dir = env::current_dir().map_err(|e| Error::Io(e))?;
    // Create the relative path to the schema.sql file
    let schema_path = current_dir.join("src/database/migrations/schema.sql");

    let schema = fs::read_to_string(schema_path)
        .map_err(|e| Error::Io(e))?;
    pool.execute(schema.as_str()).await?;
    Ok(())
}

pub async fn save_flow(pool: &SqlitePool, flow: DataModel) -> Result<(), Error> {
    initialise_schema(pool).await?;

    let query = r#"
        INSERT INTO flows (
            src_ip, src_port, dst_ip, dst_port, protocol, 
            total_bytes, total_packet_count,
            sbytes, smean, dmean, dbytes, dload, sload, dpkts, rate, dttl, spkts,
            start_time, last_updated_time, dur
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"#;

    let result = sqlx::query(query)
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
        .bind(flow.spkts)
        .bind(system_time_to_timestamp(flow.start_time))
        .bind(system_time_to_timestamp(flow.last_update_time))
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

// Helper function to convert SystemTime to Unix timestamp (i64)
fn system_time_to_timestamp(time: SystemTime) -> i64 {
    time.duration_since(SystemTime::UNIX_EPOCH)
        .map(|dur| dur.as_secs() as i64)
        .unwrap_or(0) // Fallback to 0 in case of error
}