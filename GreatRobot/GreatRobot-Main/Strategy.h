#ifndef STRATEGY_H
#define STRATEGY_H
#include "LineFollower.h"
#include "Movement.h"
#include "Timer.h"
// MARK: Libraries
//#include <Arduino.h>
// #include <Adafruit_MotorShield.h>
class Strategy{
  public:
    Strategy(LineFollower *lineFollower);
    void setup();
    void play();
    void Strategy::setTeam(int newTeam);
    void activateTimer();
    void disableTimer();
  private:
    LineFollower* lineFollower;
    enum {START, FOLLOW_LINE, STEP_FORWARD, RELEASE_PLANTS,HARDCODE_ROT1,HARDCODE_BACKWARD1,HARDCODE_ROT2,HARDCODE_BACKWARD2,END}strat_state; 
    const unsigned long LimitTime = 98000;
    unsigned long timeStartSTRAT;
    unsigned long timeStartStep;
    bool teamYellow;
    Timer timer;
    const unsigned int STEPFORWARDtime = 1500 ;
    const unsigned int RELEASEPLANTStime = 4500;
    const unsigned int HARDCODEROT1time = 3000 ;
    const unsigned int HARDCODEBACKWARD1time = 12000 ;
    const unsigned int HARDCODEROT2time = 2000 ;
    const unsigned int HARDCODEBACKWARD2time= 18000;
};

#endif
