#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H
#include <Adafruit_MotorShield.h>
#include "Motor.h"
// MARK: Libraries
//#include <Arduino.h>
// MARK: Class / Functions / Variables
class LineFollower {
public:
  // MARK: Lifecycle
  LineFollower(int IR_left_pin,int IR_right_pin, Motor &motor);
  void setup();
  bool followingLine();
private:
  // MARK: Variables
  int IR_left_PIN;
  int IR_right_PIN;
  Motor &motor;

  // Debuging Identifier
  //String debugIdentifier = "[SAMPLE MANAGER] | ";
  // Private constructor to prevent direct instantiation
};



#endif
