#ifndef COMMUNICATION_ARDUINO_LCD_H
#define COMMUNICATION_ARDUINO_LCD_H
#include <Arduino.h>

class CommunicationArduinoLCD {
  public:
  CommunicationArduinoLCD(); 
  void chooseTeam();
};

// Declare the startRUN() function
extern void startRUN();
extern int team;

#endif