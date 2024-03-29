#ifndef Motor_H
#define Motor_H
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

class Motor {
  private:
    uint8_t leftSpeed_;
    uint8_t rightSpeed_;
    uint8_t leftDirection_;
    uint8_t rightDirection_;

  public:
    Motor(); 
    void setupMotors();
    void controlMotors(uint8_t leftSpeed, uint8_t rightSpeed, uint8_t leftDirection, uint8_t rightDirection);
    void stopMotors();
};

extern int team;
extern const uint8_t LOW_SPEED;
extern const uint8_t HIGH_SPEED;
#endif