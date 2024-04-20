#include "Strategy.h"

Strategy::Strategy(LineFollower * lineFollower_PARAM)
{
  lineFollower = lineFollower_PARAM;
  strat_state = START;
}
void Strategy::setup(){
  timer.setup();
}
void Strategy::setTeam(int newTeam){
   if (newTeam == 1)
   {
    teamYellow = true;
    lineFollower->setTeam(1);
   }
   //BLUE TEAM
   else if (newTeam == 2)
   {
    teamYellow = false;
    lineFollower->setTeam(2);
   }
}
void Strategy::play()
{
  if(strat_state != START && millis()-timeStartSTRAT>= LimitTime){
    strat_state = END; 
  }
  switch(strat_state){
    case START:
    {
      timeStartSTRAT = millis();
      strat_state = FOLLOW_LINE;
    }
    case FOLLOW_LINE:
    {
      bool endline =lineFollower->followingLine();
      if(endline){
        timer.setTimer(STEPFORWARDtime);
        strat_state = STEP_FORWARD;
      }
      break;
    }
    case STEP_FORWARD:
    {
      if(timer.endOfTimer()){
        timer.setTimer(RELEASEPLANTStime);
        strat_state = RELEASE_PLANTS;
        Serial.println("ckpt:PlantZ");
        //Serial.println("stat start backward");
      }else{
        Movement::forward();
      }
      break;
    }
    case RELEASE_PLANTS:
    {
      if(timer.endOfTimer()){
        timer.setTimer(HARDCODEROT1time);
        strat_state = HARDCODE_ROT1;
      }else{
        Movement::backward();
      }
      break;
    }
    case HARDCODE_ROT1:
    {
      if(timer.endOfTimer()){
        timer.setTimer(HARDCODEBACKWARD1time);
        strat_state = HARDCODE_BACKWARD1;
      }else{
        if(teamYellow){
          Movement::turnLeft();
        }else{
          Movement::turnRight();
        }
      }
      break;
    }
    case HARDCODE_BACKWARD1:
    {
      if(timer.endOfTimer()){
        timer.setTimer(HARDCODEROT2time);
        strat_state = HARDCODE_ROT2;
      }else{
        Movement::backward();
      }
      break;
    }
    case HARDCODE_ROT2:
    {
      if(timer.endOfTimer()){
        timer.setTimer(HARDCODEBACKWARD2time);
        strat_state = HARDCODE_BACKWARD2;
      }else{
        if(teamYellow){
          Movement::turnRight();
        }else{
          Movement::turnLeft();
        }
      }
      break;
    }
    case HARDCODE_BACKWARD2:
    {
      if(timer.endOfTimer()){
        Serial.println("ckpt:FZ");
        strat_state = END;
      }else{
        Movement::backward();
      }
      break;
    }
    
    case END:
    {
      Movement::stopMovement();
      break;
    }
  }
}
void Strategy::activateTimer(){
  timer.activate();
  if(strat_state == FOLLOW_LINE){
    lineFollower->activateTimer();
  }
}
void Strategy::disableTimer(){
  timer.disable();
  if(strat_state == FOLLOW_LINE){
    lineFollower->disableTimer();
  }
}
