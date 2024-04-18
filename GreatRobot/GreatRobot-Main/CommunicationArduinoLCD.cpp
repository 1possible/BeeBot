#include "CommunicationArduinoLCD.h"
CommunicationArduinoLCD::CommunicationArduinoLCD(){
}
// CommunicationArduinoLCD
int CommunicationArduinoLCD::chooseTeam(){
  int team = 0;
  if(Serial.available()){
    String serialStr = Serial.readString();
    int sepaIndex = serialStr.indexOf(":");
    String serialCommand = serialStr.substring(0, sepaIndex);
    String serialData = serialStr.substring(sepaIndex + 1);
    if(serialCommand.indexOf("Team") != -1){
      if (serialData.indexOf("1")!=-1){
        team = 1;
        Serial.println("Team:OK");
      }
      //BLUE TEAM
      else if (serialData.indexOf("2")!=-1){
        team = 2;
        Serial.println("Team:OK");
      }
    }
  }
      
  return team;
}
