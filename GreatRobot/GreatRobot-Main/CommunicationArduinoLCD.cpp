#include "CommunicationArduinoLCD.h"
CommunicationArduinoLCD::CommunicationArduinoLCD(){
}
// CommunicationArduinoLCD
int CommunicationArduinoLCD::chooseTeam(){
  String readString;
  String Q;
  int team = 0;
  //delay(1);
  if(Serial.available() > 0){
    char c = Serial.read();
    readString += c;
  }
  Q = readString;
  //YELLOW TEAM
  if (Q == "1"){
    team = 1;
  }
  //BLUE TEAM
  else if (Q == "2"){
    team = 2;
  }
  return team;
}
