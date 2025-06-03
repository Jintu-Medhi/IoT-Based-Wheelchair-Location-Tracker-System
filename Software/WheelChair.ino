#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>

// Pin definitions for the RFID reader and buzzer
#define RST_PIN  D3     // Reset pin for the MFRC522
#define SS_PIN   D4     // Slave select pin for the MFRC522
#define BUZZER   D2     // Buzzer pin

// Create an instance of the MFRC522 class
MFRC522 mfrc522(SS_PIN, RST_PIN);  
MFRC522::MIFARE_Key key;  
ESP8266WiFiMulti WiFiMulti;  // WiFi multi-connect manager
MFRC522::StatusCode status;  // Variable to store status codes

// Block number to read data from
int blockNum = 2;  

// Buffer for reading data from the RFID block
byte bufferLen = 18; // Buffer length (16 bytes + 2 for status)
byte readBlockData[18]; // Array to hold block data

String data2; // To hold the complete URL for the HTTP request
const String data1 = "https://script.google.com/macros/s/---API key has been removed---/exec?name="; // Base URL
const String room = "&room=Floor1_Room2";  // Room identifier
void setup() 
{
  // Initialize serial communications with the PC
  Serial.begin(9600);

  // Wait for a few seconds for the user to prepare
  for (uint8_t t = 4; t > 0; t--) 
  {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    delay(1000); // Delay for 1 second
  }

  // Set the WiFi mode to Station
  WiFi.mode(WIFI_STA);
  
  // Add your WiFi credentials here
  WiFiMulti.addAP("iot", "project1234");

  // Set the buzzer pin as OUTPUT
  pinMode(BUZZER, OUTPUT);
  
  // Initialize the SPI bus for communication with the RFID reader
  SPI.begin();

  // Attempt to connect to WiFi
  Serial.println("[SETUP] Connecting to WiFi...");
  
  while (WiFiMulti.run() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500); // Wait for connection
  }
  
  // When connected, print WiFi status and IP address
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // Initialize the MFRC522 module
  mfrc522.PCD_Init();
  
  // Check for new RFID cards
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return; // Exit if no new card is detected
  }
  
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) 
  {
    return; // Exit if card selection failed
  }
  
  // Reading data from the specified block
  Serial.println(F("Reading last data from RFID..."));
  ReadDataFromBlock(blockNum, readBlockData);
  
  // Print the data read from the RFID block
  Serial.print(F("Last data in RFID:"));
  Serial.print(blockNum);
  Serial.print(F(" --> "));
  for (int j = 0; j < 16; j++)
  {
    Serial.write(readBlockData[j]); // Output the data byte by byte
  }
  Serial.println();

  // Activate the buzzer to indicate successful reading
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }
  
  // Wait for WiFi connection
  if (WiFiMulti.run() == WL_CONNECTED) 
  {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure(); // Disable SSL certificate verification for simplicity

    // Construct the complete URL for the HTTP request
    data2 = data1 + String((char*)readBlockData) + room; 
    data2.trim();
    Serial.println(data2);
    
    HTTPClient https; // Create an HTTP client
    Serial.print(F("[HTTPS] begin...\n"));
    
    if (https.begin(*client, (String)data2)) // Start connection
    {  
      Serial.print(F("[HTTPS] GET...\n"));
      int httpCode = https.GET(); // Send the GET request
    
      // Check for errors
      if (httpCode > 0) 
      {
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode); // Log the response code
      }
      else 
      {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end(); // End the HTTP connection
      delay(1000); // Delay before the next loop
    } 
    else 
    {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
}

// Function to read data from a specific block on the RFID card
void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{ 
  // Prepare the key for authentication (default key is 0xFFFFFFFFFFFF)
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  
  // Authenticate the desired data block for read access using Key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return; // Exit on authentication failure
  }
  else
  {
    Serial.println("Authentication success");
  }

  // Read data from the block
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return; // Exit on read failure
  }
  else
  {
    Serial.println("Block was read successfully");  
  }
}
