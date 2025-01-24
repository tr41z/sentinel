# Sentinel

Sentinel is a machine learning-based Intrusion Detection System (IDS) designed to provide real-time threat detection. It integrates a sniffer module (C++), a backend server (Golang), a frontend interface (React + Vite), and a machine learning module (Python). The system also includes a notification feature to alert users about detected threats.

## Features

- **Sniffer Module**: Captures and analyzes network traffic.
- **Backend Server**: Manages data flow and communication between modules.
- **Frontend Interface**: User-friendly React app for visualization and threat inspection.
- **Machine Learning Module**: Enhances threat detection using AI.
- **Notification System**: Notifies users of potential threats.

---

## Setup Instructions

### Prerequisites

Ensure the following are installed or follow steps below:

- **C++ Compiler** (e.g., GCC or MSVC)
- **CMake**
- **Golang**
- **Node.js & npm**

### Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/sentinel.git
   cd sentinel
   ```
2. Run the configuration script:
   - **Linux/Mac**:
     ```bash
     sh config.sh
     ```
     or
     
     ```bash
     bash config.sh
     ```
    - **Windows**:
      Right click on `config.bat` file and open it as administor
   
3. Build the Sniffer Module:
   ```bash
   cd sniffer-mod
   mkdir build
   cd build
   ```
    - **For Linux/Mac**:
      ```bash
      cmake ..
      make
      ```
    - **For Windows**:
      ```bash
      cmake -G "Unix Makefiles" ..
      make
      ```
4. Run the Backend Server: Open a new terminal and navigate to the src-go directory:
   ```bash
   cd sentinel/src-go
   go run main.go
   ```
5. Start the Frontend: Open another terminal in the sentinel directory:
   ```bash
   npm run dev
   ```
6. Access the Application: After starting the frontend, a URL will be displayed in the terminal (e.g., http://localhost:3000). Open it in your browser to use the app.

## Usage
- **Dashboard**: View an overview of network activity and detected threats.
- **Inspector**: Analyze specific data points for deeper insights.
- **Notifications**: Stay informed of potential threats in real-time.
