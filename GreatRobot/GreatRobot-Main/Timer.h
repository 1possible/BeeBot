#ifndef TIMER_H
#define TIMER_H
#include <Arduino.h>

class Timer {
public:
  Timer();
  void setup();
  void setTimer(int newRemainingTime);
  bool endOfTimer();
  void activate();
  void disable();
  unsigned long getElapsedTime();
private:
  bool activateVal;
  unsigned long timeLastCheck;
  unsigned long timeLimit;
  unsigned long elapsedTime;
  void update();
};

#endif