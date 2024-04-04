#include "Strategy.h"

Strategy::Strategy(LineFollower * lineFollower_PARAM)
{
  lineFollower = lineFollower_PARAM;
  strat_state = START;
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
      if(millis()-timeStartStep > 2000){
        timeStartStep= millis();
        strat_state = STEP_BACKWARD;
        //Serial.println("stat start backward");
      }else{
        RightMotor->setSpeed(225); 
        LeftMotor->setSpeed(225);
        RightMotor->run(FORWARD); 
        LeftMotor->run(FORWARD);
      }
      break;
    }
    case STEP_BACKWARD:
    {
      if(millis()-timeStartStep > 5000){
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
