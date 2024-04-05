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
  bool followingLine();
  void setup();
  void setTeam(int team);
private:
  // MARK: Variables
  bool detect_main_Right ;
  bool rotation_Right ;
  enum {RUN,DOUBLE_DETECT,  RETURN_TO_LINE_1 ,RETURN_TO_LINE_2, END} state_line;
  int IR_left_PIN;
  int IR_right_PIN;
  Motor &motor;
  const int vL = 75; //Low speed left //75
  const int VL = 255; //Hight speed left
  const int vR = 75; //Low speed right //50
  const int VR = 255; //Hight speed right
  const int v = 255; //speed forward 
  const unsigned long time_to_end = 500;
  unsigned long timeLineLength;
  // Debuging Identifier
  //String debugIdentifier = "[SAMPLE MANAGER] | ";
  //Function
  void rotation(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor);
  void moveForward(Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor);
  void followingLine_RUN(bool detectionLeft,bool detectionRigth,Adafruit_DCMotor *LeftMotor,Adafruit_DCMotor *RightMotor);
};



#endif
