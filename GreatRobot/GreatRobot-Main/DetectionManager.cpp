#include "DetectionManager.h"

DetectionManager::DetectionManager(SonarSensor* newSonarSensorN, SonarSensor* newSonarSensorNE,SonarSensor* newSonarSensorE,SonarSensor* newSonarSensorSE,SonarSensor* newSonarSensorS,SonarSensor* newSonarSensorSW,SonarSensor* newSonarSensorW,SonarSensor* newSonarSensorNW) {
  sonarSensorN = newSonarSensorN;
  sonarSensorNE = newSonarSensorNE;
  sonarSensorE = newSonarSensorE;
  sonarSensorSE = newSonarSensorSE;
  sonarSensorS = newSonarSensorS;
  sonarSensorSW = newSonarSensorSW;
  sonarSensorW = newSonarSensorW;
  sonarSensorNW = newSonarSensorNW;

}

void DetectionManager::setup(){
  sonarSensorN->setup(200,1000);                       // Sonar sensor
  sonarSensorW->setup(200,1000);
  sonarSensorE->setup(200,1000);
  sonarSensorS->setup(200,1000);
  sonarSensorNW->setup(200,1000);
  sonarSensorNE->setup(200,1000);
  sonarSensorSW->setup(200,1000);
  sonarSensorSE->setup(200,1000);
}
bool DetectionManager::detection(){
  bool val_detect = false;
  if ( Movement::moveState == Movement::MV_FORWARD ){
    if(sonarSensorN->detection()||sonarSensorNE->detection()||sonarSensorNW->detection()){
      //sonarSensorN->detection() || sonarSensorW->detection() || sonarSensorE->detection() ||sonarSensorS->detection() ||sonarSensorNE->detection() ||sonarSensorNW->detection() ||sonarSensorSE->detection()||sonarSensorSW->detection()) {
      val_detect = true;
    }
  }
  else if( Movement::moveState ==  Movement::MV_BACKWARD ){
    if(sonarSensorS->detection()||sonarSensorSE->detection()||sonarSensorSW->detection()){
      val_detect = true;
    }
  }else if(sonarSensorN->detection() || sonarSensorW->detection() || sonarSensorE->detection() ||sonarSensorS->detection() ||sonarSensorNE->detection() ||sonarSensorNW->detection() ||sonarSensorSE->detection()||sonarSensorSW->detection()){
    val_detect = true;
  }
  return val_detect;
}