// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "console")]
// #![cfg_attr(not(debug_assertions), windows_subsystem = "window")]
use tauri::{AppHandle, Manager};

use std::env;

mod commands;
mod services;
mod utils;
mod database;

fn main() {
    // Set the default log level to info
    env::set_var("RUST_LOG", "info");

    // Initialize logger
    env_logger::init();

    commands::commands::start_sniffer();

    // Initialize the Tauri app
    tauri::Builder::default()
        .setup(|app: &mut tauri::App| {
            let app_handle: AppHandle = app.handle();
            let main_window: tauri::Window = app_handle.get_window("main").unwrap();
            main_window.show().unwrap();
            Ok(())
        })
        .invoke_handler(tauri::generate_handler![start_sniffer]) // NOTE: Need to invoke on front-end side
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

#[tauri::command]
fn start_sniffer() {
    commands::commands::start_sniffer();
}

