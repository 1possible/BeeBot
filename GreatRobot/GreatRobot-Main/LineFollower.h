#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H
#include "Movement.h"
// MARK: Libraries
//#include <Arduino.h>
// MARK: Class / Functions / Variables
class LineFollower {
public:
  // MARK: Lifecycle
  LineFollower(int IR_left_pin,int IR_right_pin);
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
  const unsigned long time_to_end = 500;
  unsigned long timeLineLength;
  // Debuging Identifier
  //String debugIdentifier = "[SAMPLE MANAGER] | ";
  //Function
  void rotation();
  void followingLine_RUN(bool detectionLeft,bool detectionRigth);
};



#endif
