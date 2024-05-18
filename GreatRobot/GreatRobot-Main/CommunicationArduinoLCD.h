#ifndef COMMUNICATION_ARDUINO_LCD_H
#define COMMUNICATION_ARDUINO_LCD_H
#include <Arduino.h>
#include <ArduinoLog.h>

class CommunicationArduinoLCD {
  public:
  CommunicationArduinoLCD(); 
  int chooseTeam();
};

#endif
