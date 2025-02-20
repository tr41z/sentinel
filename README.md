# 🚨 Sentinel

Sentinel is a **machine learning-based Intrusion Detection System (IDS)** designed to provide **real-time threat detection**. It integrates the following components:

- 🕵️ **Sniffer Module** (C++): Captures and analyzes network traffic.
- ⚙️ **Backend Server** (Golang): Manages data flow and communication between modules.
- 💻 **Frontend Interface** (React + Vite): User-friendly app for visualization and threat inspection.
- 🧠 **Machine Learning Module** (Python): Enhances threat detection using AI.
- 🔔 **Notification System**: Alerts users about detected threats.

---

## 🌟 Features

- **Sniffer Module**: Captures and analyzes network traffic.
- **Backend Server**: Ensures smooth communication between modules.
- **Frontend Interface**: Provides an intuitive interface for users.
- **Machine Learning Module**: Implements AI to detect threats.
- **Notification System**: Real-time alerts to users.

---

## 🛠️ Setup Instructions

### 📋 Prerequisites

Ensure the following tools are installed:

- **C++ Compiler** (e.g., GCC or MSVC)
- **CMake**
- **Golang**
- **Node.js & npm**

or follow steps below

---

### ⚙️ Steps

1. **Clone the repository**:
   ```bash
   git clone https://github.com/your-username/sentinel.git
   cd sentinel
   ```
   
2. Run the configuration script:
   - **Linux/Mac**:
     ```bash
     sh scripts/config.sh
     ```
     or
     
     ```bash
     bash scripts/config.sh
     ```
    - **Windows**:
      Right click on `scripts/config.bat` file and open it as administor
   
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

4. Open other console and install dependencies:
   ```bash
   npm install
   ```
   
5. Run backend and frontend concurrently:
   ```bash
   npm start
   ```
**OR** manually start each:

Run the Backend Server: Open a new terminal and navigate to the src-go directory:
   ```bash
   cd sentinel/src-go
   go run main.go
   ```
Start the Frontend: Open another terminal in the sentinel directory:
   ```bash
   npm run dev
   ```
6. Access the Application: After starting the frontend, a URL will be displayed in the terminal (e.g., http://localhost:3000). Open it in your browser to use the app.

## 📊 Usage
- **Dashboard**: View an overview of network activity and detected threats.
- **Inspector**: Analyze specific data points for deeper insights.
- **Notifications**: Stay informed of potential threats in real-time.

### Learn more about approach [here!](https://github.com/tr41z/sentinel/blob/sentinel/docs/APPROACH.md)
