#![cfg_attr(not(debug_assertions), windows_subsystem = "window")]
use database::db::connect;
use database::model::DataModel;

use tauri::{AppHandle, Manager};

use std::env;

mod commands;
mod database;

fn main() {
    // Set the default log level to info
    env::set_var("RUST_LOG", "info");

    // Initialize logger
    env_logger::init();

    // Initialize the Tauri app
    tauri::Builder::default()
        .setup(|app: &mut tauri::App| {
            // // Get the current directory
            // let current_dir: std::path::PathBuf = env::current_dir().map_err(Error::Io)?;

            // // Create the relative path to the exec file
            // let exec_path: std::path::PathBuf = current_dir.join("bin/main");

            // // Start the main executable in a separate thread
            // let _handle: thread::JoinHandle<()> = thread::spawn(move || {
            //     Command::new(exec_path)
            //         .spawn()
            //         .expect("Failed to start the main executable");
            // });

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
            Err(e) => Err(format!("Failed to fetch flows: {}", e)), // handle errors
        },
        Err(e) => Err(format!("Failed to connect to the database: {}", e)), // handle connection errors
    }
}
