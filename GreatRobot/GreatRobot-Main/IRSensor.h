#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <Arduino.h>

class IRSensor {
public:
    IRSensor(int Pin);
    void setup(unsigned long newHoldTime);
    bool detection();
    void setHoldTime(int newHoldTime);
    
private:
    int pin;
    unsigned long timeLastDetection;
    unsigned int holdTime;
    bool val_detect;
};

#endif
