use sqlx::{Error, Executor, MySql, MySqlPool, Pool};

use async_std::task;

use dotenv::dotenv;
use std::{env, fs};

async fn connect() -> Result<Pool<MySql>, Error> {
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
    let schema: String = fs::read_to_string("/Users/michael/Desktop/Coding/FYP/sentinel/src-tauri/src/database/schema.sql")
        .expect("Failed to read 'schema.sql'");

    pool.execute(schema.as_str()).await?;

    Ok(())
}

#[allow(unused)]
pub async fn save_flow(pool: &MySqlPool) -> Result<(), Error> {Ok(())}

pub async fn do_test_connection() {
    let res: Result<Pool<MySql>, Error> = task::block_on(connect());

    match res {
        Err(err) => {
            println!("Cannot connect to database [{}]", err.to_string());
        }
        Ok(pool) => {
            println!("Connected to database successfully!");

            // Initialize the schema
            if let Err(err) = initialise_schema(&pool).await {
                println!("Failed to initialize schema: {}", err);
            } else {
                println!("Schema initialized successfully.");
            }
        }
    }
}