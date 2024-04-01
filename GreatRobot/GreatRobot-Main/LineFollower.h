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
  // const int vL = 75; //Low speed left //75
  // const int VL = 255; //Hight speed left
  // const int vR = 75; //Low speed right //50
  // const int VR = 255; //Hight speed right
  // const int v = 255; //speed forward 

  Motor &motor;

  // Debuging Identifier
  //String debugIdentifier = "[SAMPLE MANAGER] | ";
  // Private constructor to prevent direct instantiation
};



#endif
