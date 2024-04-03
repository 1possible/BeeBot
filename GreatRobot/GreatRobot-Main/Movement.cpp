#include "Movement.h"

Motor Movement::motor; 

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
}
