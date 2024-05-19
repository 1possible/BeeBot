#include "IRSensor.h"

IRSensor::IRSensor(int Pin) {
  pin = Pin;
}

void IRSensor::setup(unsigned long newHoldTime) {
    pinMode(pin, INPUT);
    timeLastDetection = 0;
    holdTime = newHoldTime;
}
void IRSensor::setHoldTime(int newHoldTime){
  holdTime = newHoldTime;
}

bool IRSensor::detection(){
  bool detection = (digitalRead(pin)==HIGH);
  if(detection){
    val_detect = true;
    timeLastDetection = millis();
  }
  else if(val_detect & millis()-timeLastDetection >holdTime){
    val_detect = false;
  }
  return val_detect;
}