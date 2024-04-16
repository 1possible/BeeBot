#include "Strategy.h"

Strategy::Strategy(LineFollower * lineFollower_PARAM)
{
  lineFollower = lineFollower_PARAM;
  strat_state = START;
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
        timeStartStep= millis();
        strat_state = STEP_FORWARD;
      }
      break;
    }
    case STEP_FORWARD:
    {
      if(millis()-timeStartStep > 1500){
        timeStartStep= millis();
        strat_state = RELEASE_PLANTS;
        //Serial.println("stat start backward");
      }else{
        Movement::forward();
      }
      break;
    }
    case RELEASE_PLANTS:
    {
      if(millis()-timeStartStep > 4500){
        timeStartStep= millis();
        Serial.println("ckpt:PlantZ");
        strat_state = HARDCODE_ROT1;
      }else{
        Movement::backward();
      }
      break;
    }
    case HARDCODE_ROT1:
    {
      if(millis()-timeStartStep > 3000){
        timeStartStep= millis();
        Serial.println("ckpt:PlantZ");
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
      if(millis()-timeStartStep > 12000){
        timeStartStep= millis();
        Serial.println("ckpt:PlantZ");
        strat_state = HARDCODE_ROT2;
      }else{
        Movement::backward();
      }
      break;
    }
    case HARDCODE_ROT2:
    {
      if(millis()-timeStartStep > 2000){
        timeStartStep= millis();
        Serial.println("ckpt:PlantZ");
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
      if(millis()-timeStartStep > 18000){
        timeStartStep= millis();
        Serial.println("ckpt:PlantZ");
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
