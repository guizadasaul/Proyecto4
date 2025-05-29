// WiFiManager.cpp
#include "WiFiManager.h"
#include <Arduino.h>

WiFiManager::WiFiManager(const char* ssid, const char* pass)
    : ssid(ssid), pass(pass) {}

void WiFiManager::connect() {
    WiFi.disconnect(true);
    delay(1000);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    waitForConnection();
}

void WiFiManager::waitForConnection() {
    Serial.print("Conectando a WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n✅ WiFi conectado!");
}
