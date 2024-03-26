// MARK: Libraries
#include <SPI.h>
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
}

bool LineFollower::followingLine(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor){
  bool endLine = false;
  bool detectionLeft = digitalRead(IR_left_PIN);
  bool detectionRigth = digitalRead(IR_right_PIN);
  if(detectionLeft == LOW and detectionRigth == HIGH){
    LeftMotor->setSpeed(VL);
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(vR);
    RightMotor->run(BACKWARD);
    timeLine = millis();
  }else if(detectionLeft == HIGH and detectionRigth == LOW){
    LeftMotor->setSpeed(vL);
    LeftMotor->run(BACKWARD );
    RightMotor->setSpeed(VR);
    RightMotor->run(FORWARD);
    timeLine = millis();;
  }else if(detectionLeft == LOW and detectionRigth == LOW){
    LeftMotor->setSpeed(v);
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(v);
    RightMotor->run(FORWARD);
    timeLine =millis();
  }else if(detectionLeft == HIGH and detectionRigth == HIGH and (millis()-timeLine) >250){
    LeftMotor->setSpeed(0);
    LeftMotor->run(RELEASE);
    RightMotor->setSpeed(0);
    RightMotor->run(RELEASE);
    endLine = true;
  }
  else{//HIGH HIGH without time out
    LeftMotor->setSpeed(v); 
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(v);
    RightMotor->run(FORWARD);
  }
  return endLine;
}
