#ifndef SONAR_SENSOR_H
#define SONAR_SENSOR_H

#include <Arduino.h>

class SonarSensor {
public:
    SonarSensor(byte triggerPin, byte echoPin);
    void setup(int cal_dist_detect);
    float measureDistance();
    bool detection();
    int getDistance();
    
private:
    const int _triggerPin;
    const int _echoPin;
    const unsigned long _MEASURE_TIMEOUT = 22000UL;
    const float _SOUND_SPEED = 340.0 / 1000;
    int dist_detect;
    int distance;
};

#endif
