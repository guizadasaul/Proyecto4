// ShadowReporter.h
#ifndef SHADOW_REPORTER_H
#define SHADOW_REPORTER_H

#include <ArduinoJson.h>
#include <time.h>
#include <PubSubClient.h>

class ShadowReporter {
public:
    ShadowReporter(PubSubClient& client);
    void report(bool pump, int ppm, bool fireEvent, const char* location);
private:
    PubSubClient& client;
    StaticJsonDocument<256> doc;
    char buffer[512];
    const char* updateTopic = "$aws/things/DameTuCosita/shadow/update";
    const char* mapAlertLevel(float ppm) const;
    void addTimestamp(JsonObject& obj);
};

#endif // SHADOW_REPORTER_H
