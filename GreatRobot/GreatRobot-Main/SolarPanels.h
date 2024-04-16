#ifndef SolarPanels_H
#define SolarPanels_H

#include "Motor.h"
#include "Movement.h"
#include "SonarSensor.h"

class SolarPanels {
  private:
    bool yellowTeam;
    SonarSensor* sonarSensor;
    static Motor motor;
    enum {ARM_ON, MOVE,SOLAR_PANEL_ROT, ARM_OFF,END_SOLARPANEL} solarPanel_state;
    int iterationCount;
    const int MAX_ITERATIONS = 2;
    float distanceFromSolarPanel;

  public:
    SolarPanels(SonarSensor* sonarSensor);
    void setup();
    bool play();
    void setTeam(int team);
};

//extern int ECHO_PIN_SolarPan;
//extern int TRIGGER_PIN;

#endif