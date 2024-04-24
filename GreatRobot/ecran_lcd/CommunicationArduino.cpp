#include "CommunicationArduino.h"

CommunicationArduino::CommunicationArduino(){
}
void CommunicationArduino::setup(){
  Serial.begin(9600);
}
void CommunicationArduino::sendTeam(String team){
  Serial.print("Team:"+team);
}
bool CommunicationArduino::receive(ScoreCalculator *scrCal){
  bool recVal = false;
  if(Serial.available()){
    String serialStr = Serial.readString();
    int sepaIndex = serialStr.indexOf(":");
    String serialCommand = serialStr.substring(0, sepaIndex);
    String serialData = serialStr.substring(sepaIndex + 1);
    recVal = true;
    if(serialCommand.indexOf("ckpt") != -1){
      Serial.println("cmd recu");
      cmdCkpt(serialData, scrCal);
      
    }else{
      recVal = false;
    }
  }
  return recVal;
}
void CommunicationArduino::cmdCkpt(String dataCmd, ScoreCalculator *scrCal){
  if(dataCmd.indexOf("FZ")!=-1){ //final zone
    scrCal->addFinalZone();
  }
  else if (dataCmd.indexOf("SolP")!=-1){//solarPanel
    scrCal->addOneSonarPanelToScore();
  }
  else if (dataCmd.indexOf("PlantZ")!=-1){//plant in zone
    scrCal->addPlantToScore();
  }
}