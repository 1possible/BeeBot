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
      if(millis()-timeStartStep > 1000){
        timeStartStep= millis();
        strat_state = STEP_ROT;
        //Serial.println("stat start backward");
      }else{
        Movement::forward();
      }
      break;
    }
    case STEP_ROT:
    {
      if(millis()-timeStartStep > 4000){
        timeStartStep= millis();
        strat_state = RETURN_TO_BASE;
      }else{
        if(teamYellow){ 
          Movement::turnRight();
        }else{
          Movement::turnLeft();
        }
      }
      break;
    }
    case RETURN_TO_BASE:
    {
      if(millis()-timeStartStep > 20000){
        timeStartStep= millis();
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
