#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]
use database::db::connect;
use database::model::DataModel;

use dirs::home_dir;
use dotenv::Error;
use std::path::PathBuf;
use std::thread;

use std::{env, process::Command};

use tauri::{AppHandle, Manager};

mod commands;
mod database;

use log::{error, LevelFilter};
use simplelog::{Config, WriteLogger};
use std::fs::{create_dir_all, File};

fn main() {
    // Set the default log level to info
    env::set_var("RUST_LOG", "info");

    let log_file_path: PathBuf = home_dir().unwrap().join(".sentinel/sentinel_app.log");

    // Ensure the directory exists
    if let Err(e) = create_dir_all(log_file_path.parent().unwrap()) {
        eprintln!("Error creating directory: {}", e);
        return;
    }

    // Initialize logger
    if let Err(e) = File::create(&log_file_path).map(|log_file| {
        WriteLogger::init(LevelFilter::Info, Config::default(), log_file).unwrap();
    }) {
        eprintln!("Error initializing log file: {}", e);
        return;
    }

    // Initialize the Tauri app
    tauri::Builder::default()
        .setup(|app: &mut tauri::App| {
            // Get the current directory
            let current_dir: std::path::PathBuf = env::current_dir().map_err(Error::Io)?;

            // Create the relative path to the exec file
            let exec_path: std::path::PathBuf = current_dir.join("bin/sniffer");

            // Start the main executable in a separate thread
            let _handle: thread::JoinHandle<()> = thread::spawn(move || {
                Command::new(exec_path)
                    .spawn()
                    .expect("Failed to start the main executable");
            });

            // Show the main window
            let app_handle: AppHandle = app.handle();
            let main_window: tauri::Window = app_handle.get_window("main").unwrap();
            main_window.show().unwrap();

            Ok(())
        })
        .invoke_handler(tauri::generate_handler![fetch_flows])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

#[tauri::command]
async fn fetch_flows() -> Result<Vec<DataModel>, String> {
    match connect().await {
        Ok(pool) => match commands::commands::get_flows(pool).await {
            Ok(flows) => Ok(flows), // return flows on success
            Err(e) => {
                error!("Failed to fetch flows: {}", e);
                Err(format!("Failed to fetch flows: {}", e))
            }
        },
        Err(e) => {
            error!("Failed to connect to the database: {}", e);
            Err(format!("Failed to connect to the database: {}", e))
        }
    }
}
