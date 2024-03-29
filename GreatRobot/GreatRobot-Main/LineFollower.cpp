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
bool LineFollower::followingLine(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor){
  bool endLine = false;
  bool detectionLeft = digitalRead(IR_left_PIN);
  bool detectionRigth = digitalRead(IR_right_PIN);
  switch(state_line){
    case RUN:
    {
      followingLine_RUN(detectionLeft,detectionRigth,LeftMotor,RightMotor);
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
        moveForward(LeftMotor,RightMotor);
      }
      break;
    }
    case RETURN_TO_LINE_1:
    {
      if((detect_main_Right and detectionRigth == HIGH) or (!detect_main_Right and detectionLeft == HIGH))
      {
        state_line = RETURN_TO_LINE_2;
      }
      rotation(LeftMotor, RightMotor);
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
        rotation(LeftMotor, RightMotor);
      }
      break;
    }
    case END:
    {
      endLine = false;
      LeftMotor->setSpeed(0);
      LeftMotor->run(RELEASE);
      RightMotor->setSpeed(0);
      RightMotor->run(RELEASE);
      break;
    }
  }
  return endLine;
}
void LineFollower::followingLine_RUN(bool detectionLeft,bool detectionRigth,Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor){
  if(detectionLeft == HIGH and detectionRigth == HIGH){
    timeLineLength =millis();
    state_line = DOUBLE_DETECT;
    moveForward(LeftMotor,RightMotor);
  }
  else if( (detect_main_Right and detectionRigth == HIGH) or (!detect_main_Right and detectionLeft == HIGH))
  {
    rotation(LeftMotor, RightMotor);
  }
  else{
    moveForward(LeftMotor,RightMotor);
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
void LineFollower::rotation(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor){
  if(rotation_Right){
    LeftMotor->setSpeed(VL);
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(VR);
    RightMotor->run(BACKWARD);
  }else{
    LeftMotor->setSpeed(VL);
    LeftMotor->run(BACKWARD);
    RightMotor->setSpeed(VR);
    RightMotor->run(FORWARD);
  }
}
void LineFollower::moveForward(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor){
    LeftMotor->setSpeed(v);
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(v);
    RightMotor->run(FORWARD);
}
