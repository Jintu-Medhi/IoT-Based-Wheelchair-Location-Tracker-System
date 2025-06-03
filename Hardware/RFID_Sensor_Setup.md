# 📡 RFID Sensor Setup for Smart Wheelchair Tracking System

This guide walks you through setting up the MFRC522 RFID reader module with a NodeMCU (ESP8266) microcontroller, as used in this project to detect and transmit wheelchair location data to Google Sheets via WiFi.

---

## 🔧 Components Required

- NodeMCU ESP8266
- MFRC522 RFID Reader
- RFID Tags/Cards
- Buzzer (optional for audio feedback)
- Jumper Wires
- Breadboard (optional)

---

## 🔌 Wiring Configuration

Connect the MFRC522 to the NodeMCU as follows:

| MFRC522 Pin | NodeMCU Pin |
|-------------|-------------|
| **SDA**     | D4 (GPIO2)  |
| **SCK**     | D5 (GPIO14) |
| **MOSI**    | D7 (GPIO13) |
| **MISO**    | D6 (GPIO12) |
| **IRQ**     | Not connected |
| **GND**     | GND         |
| **RST**     | D3 (GPIO0)  |
| **3.3V**    | 3.3V        |

> ⚠️ **Important:** Connect MFRC522 to **3.3V only**, not 5V.

### Buzzer Connection

| Component | NodeMCU Pin |
|-----------|-------------|
| Buzzer    | D2 (GPIO4)  |

---

## ⚙️ Software Setup

1. **Install Libraries in Arduino IDE**:
   - `MFRC522` (by GithubCommunity)
   - `ESP8266WiFi`
   - `ESP8266HTTPClient`
   - `WiFiClientSecureBearSSL`

2. **Add WiFi Credentials in Code**:
   Replace this line in your code with your WiFi details:
   ```cpp
   WiFiMulti.addAP("iot", "project1234"); // Change to your own SSID and password

Google Script URL Setup:
The RFID tag data is sent to:

    https://script.google.com/macros/s/------------------------------------/exec?name=<RFID_DATA>&room=Floor1_Room2

    Update this in the code.

✅ How It Works

    On startup:

        Connects to WiFi.

        Initializes the MFRC522 module.

    In the loop:

        Scans for RFID tags.

        Reads data from block 2 of the tag.

        Activates buzzer to indicate success.

        Sends data via HTTPS to a Google Script endpoint.

🧪 Testing Steps

    Upload the code to NodeMCU.

    Open Serial Monitor at 9600 baud.

    Power the NodeMCU and ensure it connects to WiFi.

    Scan an RFID tag.

    You should see:

        RFID block data printed to the Serial Monitor.

        Buzzer beeping.

        Data sent to Google Sheets (if the Google Script is correctly set up).

🛠️ Troubleshooting

    No response when scanning:

        Check wiring, especially SDA/SS and RST pins.

        Ensure MFRC522 is powered from 3.3V.

    WiFi not connecting:

        Double-check your SSID/password.

        Check WiFi range/signal strength.

    No data in Google Sheet:

        Test the Google Apps Script URL in a browser.

        Check Serial Monitor for HTTP status codes or SSL errors.

📌 Notes

    Only block 2 is being read. Make sure your RFID tags have valid data written to this block.

    Buzzer feedback helps confirm successful scans.

    setInsecure() disables SSL certificate verification. For production, consider using a verified certificate or HTTP fallback with caution.

