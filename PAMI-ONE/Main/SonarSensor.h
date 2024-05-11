#ifndef SONAR_SENSOR_H
#define SONAR_SENSOR_H

#include <Arduino.h>

class SonarSensor {
public:
    SonarSensor(byte triggerPin, byte echoPin);
    float measureDistance();
    bool obstacleInTheWay();
    
private:
    float distance_mm;
    const int _triggerPin;
    const int _echoPin;
    const unsigned long _MEASURE_TIMEOUT = 25000UL;
    const float _SOUND_SPEED = 340.0 / 1000;
};

#endif
