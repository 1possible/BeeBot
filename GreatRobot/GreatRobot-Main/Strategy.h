#ifndef STRATEGY_H
#define STRATEGY_H
#include "LineFollower.h"
#include "Movement.h"
// MARK: Libraries
//#include <Arduino.h>
// #include <Adafruit_MotorShield.h>
class Strategy{
  public:
    Strategy(LineFollower *lineFollower);
    void play();
    void Strategy::setTeam(int newTeam);
  private:
    LineFollower* lineFollower;
    enum {START, FOLLOW_LINE, STEP_FORWARD, STEP_ROT,RETURN_TO_BASE,END}strat_state; 
    const unsigned long LimitTime = 100000;
    unsigned long timeStartSTRAT;
    unsigned long timeStartStep;
    bool teamYellow;
};

#endif
