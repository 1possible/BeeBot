// MARK: Libraries

// MARK: Modules
#include "LineFollower.h"

// MARK: Variables
unsigned long timeLine = 0;

// MARK: Constructor
LineFollower::LineFollower(int IR_left_pin,int IR_right_pin, Motor &motor) : motor(motor) {
  // Initialization code, if needed
  IR_left_PIN = IR_left_pin;
  IR_right_PIN = IR_right_pin;
}

// MARK: LifeCycle
void LineFollower::setup() {
  pinMode(IR_left_PIN, INPUT);
  pinMode(IR_right_PIN, INPUT);
}

bool LineFollower::followingLine(){
  bool endLine = false;
  bool detectionLeft = digitalRead(IR_left_PIN);
  bool detectionRigth = digitalRead(IR_right_PIN);
  if(detectionLeft == LOW and detectionRigth == HIGH){
    motor.controlMotors(HIGH_SPEED, LOW_SPEED, FORWARD, BACKWARD);
    timeLine = millis();
  }else if(detectionLeft == HIGH and detectionRigth == LOW){
    motor.controlMotors(LOW_SPEED, HIGH_SPEED, BACKWARD, FORWARD);
    timeLine = millis();
  }else if(detectionLeft == LOW and detectionRigth == LOW){
    motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, FORWARD);
    timeLine =millis();
  }else if(detectionLeft == HIGH and detectionRigth == HIGH and (millis()-timeLine) >250){
    motor.stopMotors();
    endLine = true;
  }
  else{//HIGH HIGH without time out
    motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, FORWARD);
  }
  return endLine;
}
