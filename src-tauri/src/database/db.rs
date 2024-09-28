use sqlx::{Pool, MySql, Error, MySqlPool};

use async_std::task;

use dotenv::dotenv;
use std::env;

async fn connect() -> Result<Pool<MySql>, Error> {
    dotenv().ok();

    let username = env::var("DB_USERNAME").expect("'DB_USERNAME' must be set!");
    let password = env::var("DB_PASSWORD").expect("'DB_PASSWORD' must be set!");
    let db_name = env::var("DB_NAME").expect("'DB_NAME' must be set!");
    let db_url = env::var("DB_URL").expect("'DB_URL' must be set!");

    let connection_string = format!("mysql://{}:{}@{}/{}", username, password, db_url, db_name);
    return MySqlPool::connect(&connection_string).await;
}

pub async fn do_test_connection() {
    let res = task::block_on(connect());

    match res {
        Err(err) => {
            println!("Cannot connect to database [{}]", err.to_string());
        }
        Ok(_) => {
            println!("Connected to database successfully!");
        }
    }
}