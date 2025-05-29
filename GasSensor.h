// GasSensor.h
#ifndef GAS_SENSOR_H
#define GAS_SENSOR_H

class GasSensor {
public:
    GasSensor(int pin);
    void read();
    float getPPM() const;
    bool isFireEvent() const;
private:
    int pin;
    float ppm;
    bool fireEvent;
};

#endif // GAS_SENSOR_H
