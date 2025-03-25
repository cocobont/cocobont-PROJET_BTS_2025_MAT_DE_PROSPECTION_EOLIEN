// WindSensor.cpp
#include "WindSensor.h"

volatile unsigned int impulses = 0;

WindSensor::WindSensor(int pinAnemometer, int pinVane)
    : _pinAnemometer(pinAnemometer), _pinVane(pinVane), _nbImpulses(0), _lastTime(0) {}

void WindSensor::begin() {
    Serial.begin(115200);
    pinMode(_pinAnemometer, INPUT);
    attachInterrupt(digitalPinToInterrupt(_pinAnemometer), countImpulses, FALLING);
    Serial.println("Station météo initialisée");
    Serial.println("------------------------");
}

void IRAM_ATTR WindSensor::countImpulses() {
    impulses++;
}

void WindSensor::update() {
    unsigned long currentTime = millis();
    if (currentTime - _lastTime >= 1000) {
        _nbImpulses = impulses;
        impulses = 0;
        _lastTime = currentTime;
    }
}

int WindSensor::getWindSpeed() {
    return (_nbImpulses * 2.4); // Convert impulses to km/h
}

const char* WindSensor::getWindDirection() {
    int analogValue = analogRead(_pinVane);
    float tension = analogValue * (3.3 / 4095);
    int index = findClosestDirection(tension);
    return index >= 0 ? _directions[index] : "Inconnue";
}

int WindSensor::findClosestDirection(float tension) {
    int closestIndex = -1;
    float minDifference = 5.0;
    
    for (int i = 0; i < 8; i++) {
        float difference = abs(tension - _tensionDirections[i]);
        if (difference < minDifference && difference < _tolerance) {
            minDifference = difference;
            closestIndex = i;
        }
    }
    return closestIndex;
}