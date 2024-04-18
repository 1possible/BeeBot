#ifndef SONAR_SENSOR_H
#define SONAR_SENSOR_H

#include <Arduino.h>

class SonarSensor {
public:
    SonarSensor(byte triggerPin, byte echoPin);
    void setup(int cal_dist_detect, unsigned long newHoldTime);
    float measureDistance();
    bool detection();
    int getDistance();
    void setDistanceDetection(int newValDistDetect);
    void setHoldTime(int newHoldTime);
    
private:
    const int _triggerPin;
    const int _echoPin;
    const unsigned long _MEASURE_TIMEOUT = 22000UL;
    const float _SOUND_SPEED = 340.0 / 1000;
    int distDetect;
    int distance;
    unsigned long timeLastDetection;
    unsigned int holdTime;
    bool val_detect;
};

#endif
