// WindSensor.h
#ifndef WIND_SENSOR_H
#define WIND_SENSOR_H

#include <Arduino.h>

class WindSensor {
public:
    WindSensor(int pinAnemometer, int pinVane);
    void begin();
    void update();
    int getWindSpeed();
    const char* getWindDirection();

private:
    int _pinAnemometer;
    int _pinVane;
    volatile unsigned int _nbImpulses;
    unsigned long _lastTime;
    float _tensionDirections[8] = {2.44, 1.36, 0.17, 0.46, 0.79, 1.89, 3.15, 2.81};
    int _degreesDirections[8] = {0, 45, 90, 135, 180, 225, 270, 315};
    const char* _directions[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
    float _tolerance = 0.2;
    static void IRAM_ATTR countImpulses();
    int findClosestDirection(float tension);
};

#endif