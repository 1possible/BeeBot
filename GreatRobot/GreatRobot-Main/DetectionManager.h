#ifndef DETECTION_MANAGER_H
#define DETECTION_MANAGER_H
#include "SonarSensor.h"
#include <Arduino.h>

class DetectionManager {
public:
    DetectionManager(SonarSensor* newSonarSensorN, SonarSensor* newSonarSensorNE,SonarSensor* newSonarSensorE,SonarSensor* newSonarSensorSE,SonarSensor* newSonarSensorS,SonarSensor* newSonarSensorSW,SonarSensor* newSonarSensorW,SonarSensor* newSonarSensorNW);
    void setup();
    bool detection();
    
private:
SonarSensor* sonarSensorN;
SonarSensor* sonarSensorW;
SonarSensor* sonarSensorE;
SonarSensor* sonarSensorS;
SonarSensor* sonarSensorNW;
SonarSensor* sonarSensorNE;
SonarSensor* sonarSensorSW;
SonarSensor* sonarSensorSE;
};

#endif