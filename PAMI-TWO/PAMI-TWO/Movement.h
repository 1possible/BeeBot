#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Motor.h"

class Movement {
  private:
  static Motor motor; // Static instance of Motor
public:
  static void setup();
  enum MoveState {MV_STOP, MV_FORWARD, MV_BACKWARD, MV_ROTATION};
  static enum MoveState moveState;
  static void stopMovement();
  static void turnRight();
  static void turnLeft();
  static void forward();
  static void backward();
};

extern int notTurn;

#endif