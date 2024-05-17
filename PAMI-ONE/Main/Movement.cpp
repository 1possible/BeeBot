#include "Movement.h"

Motor Movement::motor; 
enum Movement::MoveState Movement::moveState;


void Movement::stopMovement() {
  motor.stopMotors();
}

void Movement::turnRight() {
  motor.controlMotors(72, 0, FORWARD, FORWARD);
}

void Movement::turnLeft() {
  motor.controlMotors(0, 72, FORWARD, FORWARD);
}

void Movement::forward() {
  motor.controlMotors(HIGH_SPEED, HIGH_SPEED + 19, FORWARD, FORWARD);   // +19 because left and right motor are a bit different
}