#ifndef STRATEGY_H
#define STRATEGY_H

#include "LineFollower.h"
// MARK: Libraries
//#include <Arduino.h>
#include <Adafruit_MotorShield.h>
class Strategy{
  public:
    Strategy(LineFollower *lineFollower);
    void play(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor);
  private:
    LineFollower* lineFollower;
    enum {START, FOLLOW_LINE, STEP_FORWARD, STEP_BACKWARD,END}strat_state; 
    const unsigned long LimitTime = 100000;
    unsigned long timeStartSTRAT;
    unsigned long timeStartStep;
};

#endif
