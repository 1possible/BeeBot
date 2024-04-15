#ifndef SolarPanels_H
#define SolarPanels_H

#include "Motor.h"
#include "Movement.h"
#include "SonarSensor.h"

class SolarPanels {
  private:
    SonarSensor sonarSensor;
    static Motor motor;

  public:
    SolarPanels();
    void setupSolarPanels();
    void play();
};

extern int ECHO_PIN_9;
extern int TRIGGER_PIN;

#endif