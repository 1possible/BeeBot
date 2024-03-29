#include "CommunicationArduinoLCD.h"
CommunicationArduinoLCD::CommunicationArduinoLCD(){
}
// CommunicationArduinoLCD
void CommunicationArduinoLCD::chooseTeam(){
  String readString;
  String Q;
  //delay(1);
  if(Serial.available() > 0){
    char c = Serial.read();
    readString += c;
  }
  Q = readString;
  //YELLOW TEAM
  if (Q == "1"){
    team = 1;
    startRUN();
  }
  //BLUE TEAM
  else if (Q == "2"){
    team = 2;
    startRUN();
  }
}
