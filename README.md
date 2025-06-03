# IoT-Based Wheelchair Location Tracker System

<img width="641" alt="NodeMCU_Circuit_Diagram" src="https://github.com/user-attachments/assets/8dd850d7-74e4-4386-8c90-2b32a29c52e2" />


## Advanced Indoor Positioning System for Wheelchairs

This project implements an advanced indoor positioning system designed to track the location of wheelchairs within a multi-floor hospital using IoT-based RFID technology. It enables real-time location tracking and reporting by room and floor, improving operational efficiency and enhancing patient service quality.

---

## 📌 Objectives

- **RFID Tracking**: Utilize RFID sensors and tags to provide accurate, real-time wheelchair tracking.
- **Location Reporting**: Report wheelchair locations using room and floor identifiers in a four-floor hospital.
- **User Interface**: Develop an intuitive, scalable UI to manage and visualize wheelchair data, supporting multiple devices.

---

## ⚙️ Features

- **RFID Sensors**: Detect RFID tags on wheelchairs through sensors installed at room entry and exit points.
- **Indoor Positioning**: Determine wheelchair positions using custom floor maps with floor-wise rotational adjustments.
- **UI Design**: A centralized, user-friendly interface to manage, monitor, and track wheelchair locations.
- **Floor Mapping**: Each of the four floors has a unique layout of five asymmetric rooms rotated 90° from the floor below.

---

## 🧩 Components

- **RFID Sensors**: Deployed at room entry and exit gates.
- **RFID Tags**: Attached to wheelchairs for unique identification.
- **User Interface**: Web-based or spreadsheet UI to display and manage location data.
- **Indoor Mapping**: Custom-designed floormaps for accurate room-level tracking.

---

## 🔄 Final Workflow

### 1. Initial Scan (Check-In)
- **Occupancy Check**:
  - If the wheelchair has no active session:
    - Create a new entry in the Google Sheet with current date and time.
    - Mark status as **'Occupied'** and start a 1-minute countdown for auto-logout.
  - If the wheelchair is already marked as occupied:
    - End the current session, log exit time, and mark it as **'Free'**.

- **Restricted Area Check**:
  - If scanned in **Restricted_Area**, trigger a buzzer and activate a continuous alert.
  - Alert continues until the wheelchair is scanned again.

---

### 2. Subsequent Scan Within 1 Minute
- **If scanned before auto-logout**:
  - **If still occupied**:
    - Log exit time.
    - Mark status as **'Manually logged out'** and update to **'Free'**.
  - **If already logged out**:
    - Create a new session entry with the new check-in time.

---

### 3. Automatic Logout
- After 1 minute of inactivity:
  - Log the exit time.
  - Mark the entry as **'Automatically logged out'** and update to **'Free'**.
- If manual logout has already occurred, the automatic trigger does **nothing**.

---

### 4. Multiple Wheelchairs
- Each wheelchair has its own independent countdown timer.
- Unique triggers are created for each (e.g., `triggerAutoLogout_WheelChair_1`, `triggerAutoLogout_WheelChair_2`, etc.).

---

### 5. Restricted Area Alert (Special Case)
- Scanning a wheelchair in **Restricted_Area**:
  - Triggers a continuous buzzer alert.
  - Alert stops **only when** the wheelchair is scanned again to log out or move.

---

## ✅ Conclusion

This system enables accurate, real-time tracking of wheelchairs inside hospitals, streamlining equipment usage and ensuring timely services. The scalable and intuitive UI ensures ease of management, while features like restricted area alerts and automated logout mechanisms contribute to improved operational control and safety.

---


---

## 👨‍💻 Authors

- **Jintu Medhi** — Project Design, Implementation, and Documentation

---

## 🛠️ Technologies Used

- NodeMCU (ESP8266)
- MFRC522 RFID Reader
- Google Apps Script
- Google Sheets
- Buzzer (for alerts)

---


> *Note: This project is a proof of concept aimed at improving hospital logistics. For production deployment, additional fail-safes and robust hardware integration are recommended.*

