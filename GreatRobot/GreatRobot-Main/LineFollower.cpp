// MARK: Libraries

// MARK: Modules
#include "LineFollower.h"

// MARK: Variables
unsigned long timeLine = 0;

// MARK: Constructor
LineFollower::LineFollower(int IR_left_pin,int IR_right_pin) {

  // Initialization code, if needed
  IR_left_PIN = IR_left_pin;
  IR_right_PIN = IR_right_pin;
}

// MARK: LifeCycle
void LineFollower::setup() {
  pinMode(IR_left_PIN, INPUT);
  pinMode(IR_right_PIN, INPUT);
  detect_main_Right = true;
  rotation_Right = true;
}

void LineFollower::setTeam(int team){
  if(team ==1){
    detect_main_Right = true;
    rotation_Right = true;
  }else{
    detect_main_Right = false;
    rotation_Right = false;
  }
  
}
bool LineFollower::followingLine(){
  bool endLine = false;
  bool detectionLeft = digitalRead(IR_left_PIN);
  bool detectionRigth = digitalRead(IR_right_PIN);
  switch(state_line){
    case RUN:
    {
      followingLine_RUN(detectionLeft,detectionRigth);
      break;
    }
    case DOUBLE_DETECT:
    {
      if(detectionLeft == LOW or detectionRigth == LOW){
        if(millis()- timeLineLength >= time_to_end){
          state_line = END;
        }else{
          state_line = RETURN_TO_LINE_1;
        }
      }
      else
      {
        Movement::forward();
      }
      break;
    }
    case RETURN_TO_LINE_1:
    {
      if((detect_main_Right and detectionRigth == HIGH) or (!detect_main_Right and detectionLeft == HIGH))
      {
        state_line = RETURN_TO_LINE_2;
      }
      rotation();
      break;
    }
    case RETURN_TO_LINE_2:
    {
      if((detect_main_Right and detectionRigth == LOW) or (!detect_main_Right and detectionLeft == LOW))
      {
        state_line = RUN;
      }
      else
      {
        rotation();
      }
      break;
    }
    case END:
    {
      endLine = true;
      Movement::stopMovement();
      break;
    }
  }
  return endLine;
}
void LineFollower::followingLine_RUN(bool detectionLeft,bool detectionRigth){
  if(detectionLeft == HIGH and detectionRigth == HIGH){
    timeLineLength =millis();
    state_line = DOUBLE_DETECT;
    Movement::forward();
  }
  else if( (detect_main_Right and detectionRigth == HIGH) or (!detect_main_Right and detectionLeft == HIGH))
  {
    rotation();
  }
  else{
    Movement::forward();
  }

}
/*void LineFollower::followingLine_DOUBLE_DETECT(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor){
}
void LineFollower::followingLine_RETURN_TO_LINE_1(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor){
}
void LineFollower::followingLine_RETURN_TO_LINE_2(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor){
}
void LineFollower::followingLine_END(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor){
}*/
void LineFollower::rotation(){
  if(rotation_Right){
    Movement::turnRight();
  }else{
    Movement::turnLeft();
  }
}

