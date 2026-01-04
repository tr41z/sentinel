# Sentinel

Sentinel is a **machine learning-powered Intrusion Detection System (IDS)** designed for **real-time threat detection**. It combines multiple components for efficient monitoring and alerting:

- **Sniffer Module** (C++): Captures and inspects network traffic.
- **Backend Server** (Golang): Manages data flow and communication.
- **Frontend Interface** (React + Vite): Intuitive dashboard for threat visualization.
- **Machine Learning Module** (Python): AI-driven threat detection.
- **Notification System**: Instantly alerts users about detected threats.

---

![Dashboard](https://github.com/user-attachments/assets/53423096-14f7-45c4-b84a-3968601b6ff4)

---

## Key Features

- **Network Sniffer**: Monitors and analyzes live network traffic.
- **AI-Powered Detection**: Uses machine learning to identify threats.
- **User-Friendly Dashboard**: Visualize and inspect security events easily.
- **Real-Time Alerts**: Get instant notifications on potential threats.
- **Modular & Scalable**: Built with separate components for flexibility.

---

## Setup Guide

### Prerequisites

Ensure you have the following installed:

- **C++ Compiler** (e.g., GCC, MSVC)
- **CMake**
- **Golang**
- **Node.js & npm**

Alternatively, follow the steps below.

---

### Installation Steps

1. **Clone the repository**:
   ```bash
   git clone https://github.com/your-username/sentinel.git
   cd sentinel
   ```
   
2. Run the configuration script:
   - **Linux/Mac**:
     ```bash
     sh setup.sh
     ```
     or
     
     ```bash
     bash setup.sh
     ```
    - **Windows**:
      Right click on `setup.bat` file and open it as administor

4. Open other console and install dependencies:
   ```bash
   npm install
   ```
   
5. Run backend and frontend concurrently:
   ```bash
   npm start
   ```
   
6. Access the Application: After starting the frontend, a URL will be displayed in the terminal (e.g., http://localhost:3000). Open it in your browser to use the app.

## Usage
- **Dashboard**: View an overview of network activity and detected threats.
- **Inspector**: Analyze specific data points for deeper insights.
- **Notifications**: Stay informed of potential threats in real-time.

### Learn more about approach [here!](https://github.com/tr41z/sentinel/blob/sentinel/docs/APPROACH.md)
