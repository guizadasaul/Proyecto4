// GasSensor.cpp
#include "GasSensor.h"
#include <Arduino.h>

GasSensor::GasSensor(int pin) : pin(pin), ppm(0), fireEvent(false) {}

void GasSensor::read() {
    int raw = analogRead(pin);
    ppm = raw * (5.0 / 4095.0) * 100.0;
    fireEvent = (ppm > 200.0); // Umbral crítico a 200 ppm
}

float GasSensor::getPPM() const { return ppm; }
bool GasSensor::isFireEvent() const { return fireEvent; }
