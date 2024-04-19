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
  sonarSensorN->setup(200,750);                       // Sonar sensor
  sonarSensorW->setup(200,750);
  sonarSensorE->setup(200,750);
  sonarSensorS->setup(200,750);
  sonarSensorNW->setup(200,750);
  sonarSensorNE->setup(200,750);
  sonarSensorSW->setup(200,750);
  sonarSensorSE->setup(200,750);
}
bool DetectionManager::detection(){
  bool val_detect = false;
  if (sonarSensorN->detection() || sonarSensorW->detection() || sonarSensorE->detection() ||sonarSensorS->detection() ||sonarSensorNE->detection() ||sonarSensorNW->detection() ||sonarSensorSE->detection()||sonarSensorSW->detection()) {
    val_detect = true;
  }
  return val_detect;
}