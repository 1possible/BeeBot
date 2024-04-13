#include "SolarPanels.h"
Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(); 
Adafruit_DCMotor *TriggerMotor;


SolarPanels::SolarPanels(){

}

void SolarPanels::setupSolarPanels() {
  AFMS2.begin();
  TriggerMotor = AFMS2.getMotor(1);
}

// Déploie le bras
// (
// Le robot Avance
// If le capteur sonore detect panneau solaire
// 	Tourne la roue
// 	Delay
// Le robot Avance
// ) x3