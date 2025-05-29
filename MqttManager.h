// MqttManager.h
#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class MqttManager {
public:
    MqttManager(WiFiClientSecure& secureClient, const char* broker, int port, const char* clientId);
    void begin();
    bool ensureConnection();
    void loop();
    void publish(const char* topic, const char* payload);
    void subscribe(const char* topic);
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    PubSubClient& client() { return client_; }
private:
    PubSubClient client_;
    const char* broker;
    int port;
    const char* clientId;
};

#endif // MQTT_MANAGER_H
