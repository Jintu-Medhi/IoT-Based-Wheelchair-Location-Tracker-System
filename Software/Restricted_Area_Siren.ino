#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>

#define RST_PIN  D3     // Reset pin for MFRC522
#define SS_PIN   D4     // Slave select pin for MFRC522
#define BUZZER   D2     // Buzzer pin

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Instance of the MFRC522 class
MFRC522::MIFARE_Key key;  
ESP8266WiFiMulti WiFiMulti;         // Multi WiFi connection manager
MFRC522::StatusCode status;        

int blockNum = 2;                   // Block number to read from RFID
byte bufferLen = 18;                // Buffer length (2 bytes more than block size)
byte readBlockData[18];             // Buffer for read data

String data2;                       // URL for sending data
const String data1 = "https://script.google.com/macros/s/---API key has been removed---/exec?name=";
const String room = "&room=Restricted_Area";  // Room info

bool buzzerState = false;           // Buzzer state (false = off, true = on)
bool buzzerActive = false;          // Continuous buzzer state
unsigned long previousMillis = 0;   // Timer for buzzer sound
const long interval = 500;          // Interval for buzzer high-low sound

void setup() 
{
  Serial.begin(9600);               // Initialize serial communication
  Serial.println();

  // Countdown for setup
  for (uint8_t t = 4; t > 0; t--) 
  {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);               // Set WiFi mode to station
  WiFiMulti.addAP("iot", "project1234"); // Add WiFi credentials

  pinMode(BUZZER, OUTPUT);           // Set buzzer pin as output
  SPI.begin();                       // Initialize SPI bus
}

void loop()
{
  mfrc522.PCD_Init();                // Initialize MFRC522 module

  // Wait for a new card
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    // Check if the buzzer is active and handle continuous buzzer
    if (buzzerActive) {
      handleContinuousBuzzer();
    }
    return; // No new card detected
  }
  
  // Select the card
  if (!mfrc522.PICC_ReadCardSerial()) 
  {
    return; // Failed to read card serial
  }

  // Read data from the specified block
  Serial.println(F("Reading last data from RFID..."));
  ReadDataFromBlock(blockNum, readBlockData);
  
  // Print the read data
  Serial.print(F("Last data in RFID:"));
  Serial.print(blockNum);
  Serial.print(F(" --> "));
  for (int j = 0; j < 16; j++)
  {
    Serial.write(readBlockData[j]); // Print each byte read
  }
  Serial.println();

  toggleBuzzer();                    // Toggle buzzer state on card scan
  // Buzzer will start immediately here

  // Check WiFi connection
  if (WiFiMulti.run() == WL_CONNECTED) 
  {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure(); // Bypass SSL certificate verification

    // Prepare the URL with data
    data2 = data1 + String((char*)readBlockData) + room; // Append room info
    data2.trim();
    Serial.println(data2);
    
    HTTPClient https;           // Create HTTP client
    if (https.begin(*client, (String)data2)) // Start connection
    {  
      int httpCode = https.GET(); // Send GET request
      
      // Check response
      if (httpCode > 0) 
      {
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      }
      else 
      {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end(); // End HTTP connection
      delay(1000); // Delay before next loop
    } 
    else 
    {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
}

void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{ 
  // Prepare the key for authentication
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF; // Default key
  }
  
  // Authenticate block for read access using Key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  
  // Read data from the block
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  Serial.println("Block was read successfully");  
}

void toggleBuzzer() {
  buzzerState = !buzzerState;  // Toggle the buzzer state
  
  if (buzzerState) {
    buzzerActive = true;  // Start continuous buzzer sound
    Serial.println("Buzzer ON - Continuous sound started");
    digitalWrite(BUZZER, HIGH); // Start buzzer sound immediately
  } else {
    buzzerActive = false; // Stop continuous buzzer sound
    digitalWrite(BUZZER, LOW);  // Ensure buzzer is off
    Serial.println("Buzzer OFF - Continuous sound stopped");
  }
}

void handleContinuousBuzzer() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Toggle buzzer on/off in intervals
    int buzzerState = digitalRead(BUZZER);
    digitalWrite(BUZZER, !buzzerState);
  }
}
