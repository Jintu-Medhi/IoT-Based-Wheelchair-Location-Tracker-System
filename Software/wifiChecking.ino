#include <ESP8266WiFi.h>

const char* ssid = "iot";
const char* password = "project1234";

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    Serial.println("Connecting...");

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFailed to connect");
    }
}

void loop() {
}
