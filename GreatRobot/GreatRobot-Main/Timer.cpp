#include "Timer.h"

Timer::Timer() {
}

void Timer::setup(){
  activateVal = true;
  timeLimit = 0;
  timeLastCheck = millis();
}
void Timer::setTimer(int newTimeLimit){
  elapsedTime = 0;
  timeLimit = newTimeLimit;
  timeLastCheck = millis();
}
bool Timer::endOfTimer(){
  update();
  bool endTime = true;
  if(timeLimit > elapsedTime){
    endTime = false;
  }
  return endTime;
}
void Timer::update(){
  if(activateVal){
    unsigned long timeNow = millis();
    elapsedTime += timeNow - timeLastCheck;
    timeLastCheck = timeNow;
  }
}
unsigned long Timer::getElapsedTime(){
  update();
  return elapsedTime;
}
void Timer::activate(){
  activateVal = true;
  timeLastCheck = millis();
}
void Timer::disable(){
  update();
  activateVal = false;
}