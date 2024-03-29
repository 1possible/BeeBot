#include "Motor.h"


Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *RightMotor;
Adafruit_DCMotor *LeftMotor;

Motor::Motor(){
}

// Function to initialize motors
void Motor::setupMotors() {
  AFMS.begin();
  RightMotor = AFMS.getMotor(2);
  LeftMotor = AFMS.getMotor(3);
}

// Function to control the motors
void Motor::controlMotors(uint8_t leftSpeed, uint8_t rightSpeed, uint8_t leftDirection, uint8_t rightDirection) {
  LeftMotor->setSpeed(leftSpeed); 
  LeftMotor->run(leftDirection);
  RightMotor->setSpeed(rightSpeed);
  RightMotor->run(rightDirection);
}

// Function to stop motors
void Motor::stopMotors() {
  LeftMotor->setSpeed(0); 
  LeftMotor->run(RELEASE);
  RightMotor->setSpeed(0);
  RightMotor->run(RELEASE);
}