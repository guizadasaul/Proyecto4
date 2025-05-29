// WiFiManager.h
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
public:
    WiFiManager(const char* ssid, const char* pass);
    void connect();
private:
    const char* ssid;
    const char* pass;
    void waitForConnection();
};

#endif // WIFI_MANAGER_H
