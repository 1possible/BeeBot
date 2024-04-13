#ifndef SolarPanels_H
#define SolarPanels_H
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

class SolarPanels {
  public:
    SolarPanels(); 
    void setupSolarPanels();
};
#endif