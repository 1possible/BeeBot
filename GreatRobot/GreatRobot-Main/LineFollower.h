#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H
#include <Adafruit_MotorShield.h>
// MARK: Libraries
//#include <Arduino.h>
// MARK: Class / Functions / Variables
class LineFollower {
public:
  // MARK: Lifecycle
  LineFollower(int IR_left_pin,int IR_right_pin);
  void setup();
  bool followingLine(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor);
private:
  // MARK: Variables
  int IR_left_PIN;
  int IR_right_PIN;
  const int vL = 75; //Low speed left //75
  const int VL = 250; //Hight speed left
  const int vR = 75; //Low speed right //50
  const int VR = 250; //Hight speed right
  const int v = 250; //speed forward 
  // Debuging Identifier
  //String debugIdentifier = "[SAMPLE MANAGER] | ";
  // Private constructor to prevent direct instantiation
};



#endif
