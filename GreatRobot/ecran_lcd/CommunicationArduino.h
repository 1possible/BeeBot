#ifndef Communication_Arduino_H
#define Communication_Arduino_H
#include "Arduino.h"
#include "ScoreCalculator.h"
#include <ArduinoLog.h>

class CommunicationArduino {
  public:
    CommunicationArduino();
    void setup();
    void sendTeam(String team);
    bool receive(ScoreCalculator *scrCal);
  private:
    //String name = "ARDL";
    //String partTeam = "Team"; 
    //String partMotor = "Move";
    void cmdCkpt(String dataCmd, ScoreCalculator *scrCal);

};
#endif