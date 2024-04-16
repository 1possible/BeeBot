#include "SolarPanels.h"

Motor SolarPanels::motor; 
SolarPanels::SolarPanels(SonarSensor* sonarsensor_PARAM) {
  sonarSensor = sonarsensor_PARAM;
}

void SolarPanels::setup() {
  iterationCount = 0;
  solarPanel_state = ARM_ON;
  yellowTeam = true;
}
void SolarPanels::setTeam(int team){
  if(team == 1){
    yellowTeam = true;
  }else{
    yellowTeam = false;
  }
}

// Define an enumeration for different states
/*enum SolarPanelState {
    STATE_FORWARD,
    STATE_DETECT_SOLAR_PANEL,
    STATE_TURN_SOLAR_PANEL
};*/

//olarPanelState currentState = STATE_FORWARD;
/*int iterationCount = 0;
const int MAX_ITERATIONS = 3;
float distanceFromSolarPanel;*/

bool SolarPanels::play() {
  bool endSolP = false;
  switch(solarPanel_state){
    case ARM_ON:
    {
      motor.openSolarPanelArm();
      solarPanel_state = MOVE;
      break;
    }
    case MOVE:
    {
      Serial.println("run back");
      if(yellowTeam){
        Movement::forward();
      }else{
        Movement::backward();
      }
      int distanceFromSolarPanel = sonarSensor->measureDistance();
      // If solar panel is detected, transition to STATE_TURN_SOLAR_PANEL
      if ( distanceFromSolarPanel <= 200) {//distanceFromSolarPanel >= 120 &&
        solarPanel_state = SOLAR_PANEL_ROT;
      }
      break;
    }
    case SOLAR_PANEL_ROT:
    {
      Movement::stopMovement();
      motor.turnSolarPanel();
      Serial.print("ckpt:SolP");//for score in the LCD arduino
      iterationCount ++;
      if(iterationCount <MAX_ITERATIONS ){
        iterationCount ++;
        solarPanel_state = MOVE;
      }else{
        solarPanel_state = ARM_OFF;
      }
      break;
    }
    case ARM_OFF:
    {
      motor.closeSolarPanelArm();
      endSolP = true;
      break;
    }
    case END_SOLARPANEL:
    {
      Serial.println("end Solar");
      endSolP = true;
      break;
    }
  }
  return endSolP;
}
