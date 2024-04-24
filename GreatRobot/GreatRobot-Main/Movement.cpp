#include "Movement.h"

Motor Movement::motor; 
enum Movement::MoveState Movement::moveState;

void Movement::setup(){
  moveState= MV_STOP;
}

void Movement::dodgeRight() {
  motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, BACKWARD);
  delay(3955);
  motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, FORWARD);
}

void Movement::dodgeLeft() {
  motor.controlMotors(HIGH_SPEED, HIGH_SPEED, BACKWARD, FORWARD);
  delay(3800);
  motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, FORWARD);
}

void Movement::stopMovement() {
  motor.stopMotors();
  moveState= MV_STOP;
}

void Movement::turnRight() {
  motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, BACKWARD);
  moveState= MV_ROTATION;

}

void Movement::turnLeft() {
  motor.controlMotors(HIGH_SPEED, HIGH_SPEED, BACKWARD, FORWARD);
  moveState= MV_ROTATION;
}

void Movement::forward() {
  motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, FORWARD);
  moveState= MV_FORWARD;
}

void Movement::backward() {
  motor.controlMotors(HIGH_SPEED, HIGH_SPEED, BACKWARD, BACKWARD);
  moveState= MV_BACKWARD;
}