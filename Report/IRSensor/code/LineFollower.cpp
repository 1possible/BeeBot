// MARK: Modules
#include "LineFollower.h"

// MARK: Constructor
LineFollower::LineFollower(int IR_left_pin,int IR_right_pin): IRsensorLeft(IR_left_pin),IRsensorRight(IR_right_pin) {
}

// MARK: LifeCycle
void LineFollower::setup() {
  IRsensorLeft.setup(0);
  IRsensorRight.setup(0);
  detect_main_Right = true;
  rotation_Right = true;
  timer.setup();
}

void LineFollower::setTeam(int team){
  if(team ==1){
    detect_main_Right = true;
    rotation_Right = true;
    IRsensorLeft.setHoldTime(10);
  }else{
    detect_main_Right = false;
    rotation_Right = false;
    IRsensorRight.setHoldTime(10);
  }
  
}
bool LineFollower::followingLine(){
  bool endLine = false;
  bool detectionLeft = IRsensorLeft.detection();
  bool detectionRigth = IRsensorRight.detection();
  switch(state_line){
    case RUN:
    {
      followingLine_RUN(detectionLeft,detectionRigth);
      break;
    }
    case DOUBLE_DETECT:
    {
      if(!detectionLeft or !detectionRigth){
        if(timer.endOfTimer()){
          state_line = END;
        }
        else{
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
      if((detect_main_Right and detectionRigth) or (!detect_main_Right and detectionLeft))
      {
        state_line = RETURN_TO_LINE_2;
      }
      rotation();
      break;
    }
    case RETURN_TO_LINE_2:
    {
      if((detect_main_Right and !detectionRigth) or (!detect_main_Right and !detectionLeft))
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
  if(detectionLeft and detectionRigth){
    timer.setTimer(timeToEnd);
    state_line = DOUBLE_DETECT;
    Movement::forward();
  }
  else if( (detect_main_Right and detectionRigth) or (!detect_main_Right and detectionLeft))
  {
    rotation();
  }
  else{
    Movement::forward();
  }

}

void LineFollower::rotation(){
  if(rotation_Right){
    Movement::turnRight();
  }else{
    Movement::turnLeft();
  }
}
void LineFollower::activateTimer(){
  timer.activate();
}
void LineFollower::disableTimer(){
  timer.disable();
}

