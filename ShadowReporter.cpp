// ShadowReporter.cpp
#include "ShadowReporter.h"
#include <Arduino.h>

ShadowReporter::ShadowReporter(PubSubClient& client) : client(client) {}

void ShadowReporter::report(bool pump, int ppm, bool fireEvent, const char* location) {
    doc.clear();
    JsonObject reported = doc["state"].createNestedObject("reported");
    reported["bomba"] = pump ? 1 : 0;
    reported["gas_ppm"] = ppm;
    reported["evento_incendio"] = fireEvent;
    reported["nivel_alerta"] = mapAlertLevel(ppm);
    reported["ubicacion"] = location;
    addTimestamp(reported);
    serializeJson(doc, buffer);
    client.publish(updateTopic, buffer);
    Serial.println(String("Publicado shadow: ") + buffer);
}

const char* ShadowReporter::mapAlertLevel(float ppm) const {
    return (ppm > 200.0) ? "critico" : "bajo";
}

void ShadowReporter::addTimestamp(JsonObject& obj) {
    time_t now = time(nullptr);
    struct tm* ti = gmtime(&now);
    char iso[25];
    strftime(iso, sizeof(iso), "%Y-%m-%dT%H:%M:%SZ", ti);
    obj["timestamp"] = iso;
}
