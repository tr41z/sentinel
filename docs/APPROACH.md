# 🔎 Approach of Sentinel

# Project Overview

## Project Approach

To achieve the goal of developing a high-performance, real-time reconnaissance attack detection system, the following structured approach will be employed:

### 1. Data Collection
The data collection process will involve the following steps:
- Set up **3-4 victim machines** and **1 attacker machine** in controlled environments.
- Deploy the **sniffer module** on all victim machines.
- The attacker machine will periodically perform various reconnaissance attacks on the victim machines to generate **malicious data**.
- Simultaneously, typical benign activities such as file downloading, web browsing, and video calls will be performed on the victim machines to collect **normal traffic data**.
- To ensure diversity in the dataset, the data collection process will be conducted in **three different physical locations**.

### 2. Packet Sniffer Development
A **C++/C-based network packet sniffer** will be developed to:
- Capture and analyze network traffic with **low latency**.
- Enable efficient data collection and ensure timely processing of network packets.

### 3. Machine Learning Model Development
A **Python-based machine learning module** will be implemented to:
- Develop **classification algorithms** capable of identifying malicious attack patterns with high accuracy.
- Use the collected dataset for training and testing.

### 4. Database Design
An **SQLite database** will be created to:
- Store captured network traffic data and detection results.
- Facilitate historical analysis and support decision-making processes.

### 5. Frontend Development
A **React-based web application** will be developed to:
- Provide a user-friendly interface for **real-time traffic visualization**.
- Allow users to manage detection alerts and interact with the system.

### 6. Backend Development
A **Golang-based backend service** will be developed to:
- Manage the **sniffer module** and **machine learning module** executables.
- Allow users to control module execution (e.g., adjust working or analysis intervals).
- Expose database-stored data to the frontend for further visualization and interaction.

### 7. Component Integration
To ensure a fully functional system:
- Components such as the C++/C sniffer, Python machine learning module, Golang backend, and React frontend will be **seamlessly integrated**.
- APIs and communication protocols will be established for smooth data exchange between components.
