#![cfg_attr(not(debug_assertions), windows_subsystem = "console")]
use tauri::{AppHandle, Manager};
use std::env;
use std::process::Command;
use std::thread;

mod commands;
mod services;
mod utils;
mod database;

fn main() {
    // Set the default log level to info
    env::set_var("RUST_LOG", "info");

    // Initialize logger
    env_logger::init();

    // Run the sniffer immediately
    commands::commands::start_sniffer();

    // Initialize the Tauri app
    tauri::Builder::default()
        .setup(|app| {
            // Start the main executable in a separate thread
            let _handle = thread::spawn(move || {
                Command::new("/Users/michael/Desktop/Coding/FYP/sentinel_api/main") // NOTE: switch to dynamic
                    .spawn()
                    .expect("Failed to start the main executable");
            });

            // Show the main window
            let app_handle: AppHandle = app.handle();
            let main_window = app_handle.get_window("main").unwrap();
            main_window.show().unwrap();

            Ok(())
        })
        .invoke_handler(tauri::generate_handler![start_sniffer]) // NOTE: invoke on front-end
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

#[tauri::command]
fn start_sniffer() {
    commands::commands::start_sniffer();
}
