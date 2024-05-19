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
    Log.trace("Strat : set team on YELLOW" CR);
    teamYellow = true;
    lineFollower->setTeam(1);
   }
   //BLUE TEAM
   else if (newTeam == 2)
   {
    Log.trace("Strat : set team on BLUE" CR);
    teamYellow = false;
    lineFollower->setTeam(2);
   }
}
void Strategy::play()
{
  if(strat_state != START && millis()-timeStartSTRAT>= LimitTime){
    Log.notice("Strat : new state : END" CR);
    strat_state = END; 
  }
  switch(strat_state){
    case START:
    {
      Log.notice("Strat : new state : FOLLOW_LINE" CR);
      timeStartSTRAT = millis();
      strat_state = FOLLOW_LINE;
    }
    case FOLLOW_LINE:
    {
      bool endline =lineFollower->followingLine();
      if(endline){
        Log.notice("Strat : new state : STEP_FORWARD" CR);
        timer.setTimer(STEPFORWARDtime);
        strat_state = STEP_FORWARD;
      }
      break;
    }
    case STEP_FORWARD:
    {
      if(timer.endOfTimer()){
        Log.notice("Strat : new state : RELEASE_PLANTS" CR);
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
        Log.notice("Strat : new state : HARDCODE_ROT1" CR);
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
        Log.notice("Strat : new state : HARDCODE_BACKWARD1" CR);
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
        Log.notice("Strat : new state : HARDCODE_ROT2" CR);
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
        Log.notice("Strat : new state : HARDCODE_BACKWARD2" CR);
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
        Log.notice("Strat : new state : END" CR);
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
  Log.trace("Strat : activate timer" CR);
  timer.activate();
  if(strat_state == FOLLOW_LINE){
    lineFollower->activateTimer();
  }
}
void Strategy::disableTimer(){
  Log.trace("Strat : disable timer" CR);
  timer.disable();
  if(strat_state == FOLLOW_LINE){
    lineFollower->disableTimer();
  }else if(strat_state == STEP_FORWARD){
    timer.setTimer(RELEASEPLANTStime);
    strat_state = RELEASE_PLANTS;
    Serial.println("ckpt:PlantZ");
    Movement::moveState = Movement::MV_BACKWARD;
  }
}
