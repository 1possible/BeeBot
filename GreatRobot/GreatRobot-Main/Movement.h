#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Motor.h"

class Movement {
  private:
  static Motor motor; // Static instance of Motor
public:
  static void dodgeRight();
  static void dodgeLeft();
  static void stopMovement();
};

#endif