#include "Movement.h"

Motor Movement::motor; 
enum Movement::MoveState Movement::moveState;


void Movement::stopMovement() {
  motor.stopMotors();
}

void Movement::turnRight() {
  motor.controlMotors(65+85*notTurn, 0, FORWARD, FORWARD);
}

void Movement::turnLeft() {
  motor.controlMotors(0, 67+90*notTurn, FORWARD, FORWARD);
}

void Movement::forward() {
  motor.controlMotors(HIGH_SPEED - 2, HIGH_SPEED, FORWARD, FORWARD);   // -2 because left and right motor are a bit different
}