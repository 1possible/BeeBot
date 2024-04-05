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
void Strategy::play(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor)
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
      bool endline =lineFollower->followingLine(LeftMotor,RightMotor);
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
        RightMotor->setSpeed(225); 
        LeftMotor->setSpeed(225);
        RightMotor->run(FORWARD); 
        LeftMotor->run(FORWARD);
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
          RightMotor->setSpeed(225); 
          LeftMotor->setSpeed(225);
          RightMotor->run(FORWARD); 
          LeftMotor->run(BACKWARD);
        }else{
          RightMotor->setSpeed(225); 
          LeftMotor->setSpeed(225);
          RightMotor->run(BACKWARD); 
          LeftMotor->run(FORWARD);
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
        RightMotor->setSpeed(225); 
        LeftMotor->setSpeed(225);
        RightMotor->run(BACKWARD); 
        LeftMotor->run(BACKWARD);
      }
      break;
    }
    case END:
    {
      RightMotor->run(RELEASE); 
      LeftMotor->run(RELEASE);
      RightMotor->setSpeed(0); 
      LeftMotor->setSpeed(0);
      break;
    }
  }
}
