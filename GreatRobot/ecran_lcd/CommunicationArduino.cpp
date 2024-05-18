#include "CommunicationArduino.h"

CommunicationArduino::CommunicationArduino(){
}
void CommunicationArduino::setup(){
  Serial.begin(9600);
}
void CommunicationArduino::sendTeam(String team){
  Serial.print("Team:"+team);
  Log.trace("COMM : send the team on serial : %s" CR,team.c_str());
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
      cmdCkpt(serialData, scrCal);
      
    }else{
      Log.warning("COMM : unexpected serial message" CR);
      recVal = false;
    }
  }
  return recVal;
}
void CommunicationArduino::cmdCkpt(String dataCmd, ScoreCalculator *scrCal){
  if(dataCmd.indexOf("FZ")!=-1){ //reach the final zone
    Log.notice  ("COMM : serial command 'ckpt:FZ' received" CR);
    scrCal->addFinalZone();
  }
  else if (dataCmd.indexOf("SolP")!=-1){//solarPanel
    Log.notice("COMM : serial command 'ckpt:SolP' received" CR);
    scrCal->addOneSonarPanelToScore();
  }
  else if (dataCmd.indexOf("PlantZ")!=-1){//plant in zone
    Log.notice ("COMM : serial command 'ckpt:PlantZ' received" CR);
    scrCal->addPlantToScore();
  }else{
    Log.warning("COMM : unexpected ckpt command" CR);
  }
}