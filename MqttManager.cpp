// MqttManager.cpp
#include "MqttManager.h"
#include <Arduino.h>

MqttManager::MqttManager(WiFiClientSecure& secureClient, const char* broker, int port, const char* clientId)
    : client_(secureClient), broker(broker), port(port), clientId(clientId) {}

void MqttManager::begin() {
    client_.setServer(broker, port);
}

bool MqttManager::ensureConnection() {
    if (!client_.connected()) {
        Serial.print("Intentando conectar a MQTT...");
        if (client_.connect(clientId)) {
            Serial.println("conectado");
            return true;
        } else {
            Serial.print("falló, rc=");
            Serial.print(client_.state());
            Serial.println(". Reintentando en 5 segundos");
            delay(5000);
            return false;
        }
    }
    return true;
}

void MqttManager::loop() {
    client_.loop();
}

void MqttManager::publish(const char* topic, const char* payload) {
    client_.publish(topic, payload);
}

void MqttManager::subscribe(const char* topic) {
    client_.subscribe(topic);
}

void MqttManager::setCallback(MQTT_CALLBACK_SIGNATURE) {
    client_.setCallback(callback);
}
