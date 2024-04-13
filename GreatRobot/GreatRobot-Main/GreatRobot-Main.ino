//GreatRobot code
#include <Wire.h>
#include <Arduino.h>
#include "CommunicationArduinoLCD.h"
#include "Motor.h"
#include "SonarSensor.h"
#include "EncoderLogic.h"
#include "LineFollower.h"
#include "Movement.h"
#include "Strategy.h"

// DEFINE PINS
//const int encoR_PIN = 2;              // Encoder
//const int encoL_PIN = 3;
const int IR_left_PIN = 9;            // IR sensor
const int IR_right_PIN = 10;
const int TRIGGER_PIN = 52;           // Pin sonor sensor
const int ECHO_PIN_N = 44;
const int ECHO_PIN_NE = 45;  
const int ECHO_PIN_E = 46;
const int ECHO_PIN_SE = 47; 
const int ECHO_PIN_S = 48; 
const int ECHO_PIN_SW = 49; 
const int ECHO_PIN_W = 50; 
const int ECHO_PIN_NW = 51; 
const int start_switch_PIN = 11;      // Starter switch (cordon)


// DEFINE INSTANCES FOR CLASSES
CommunicationArduinoLCD communicationArduinoLCD;
Motor motor;
//EncoderLogic encoderLogic(encoR_PIN, encoL_PIN);
LineFollower lineFollower = LineFollower(IR_left_PIN, IR_right_PIN);
Strategy strategy = Strategy(&lineFollower);
SonarSensor sonarSensorN(TRIGGER_PIN, ECHO_PIN_N);
SonarSensor sonarSensorW(TRIGGER_PIN, ECHO_PIN_NE);
SonarSensor sonarSensorE(TRIGGER_PIN, ECHO_PIN_E);
SonarSensor sonarSensorS(TRIGGER_PIN, ECHO_PIN_SE);
SonarSensor sonarSensorNW(TRIGGER_PIN, ECHO_PIN_S);
SonarSensor sonarSensorNE(TRIGGER_PIN, ECHO_PIN_SW);
SonarSensor sonarSensorSW(TRIGGER_PIN, ECHO_PIN_W);
SonarSensor sonarSensorSE(TRIGGER_PIN, ECHO_PIN_NW);


// DEFINE CONSTANTS
const uint8_t LOW_SPEED = 75;                                 // motor speeds
const uint8_t HIGH_SPEED = 250;
int team = 1;                                                 // Lcd team       // 1 YELLOW Team 2 BLUE
unsigned long timeNow;
unsigned long timeStartRUN;
float distance1 = 0.0;                                        // Sonar sensor
float distance2 = 0.0;
float distance3 = 0.0;
float distance4 = 0.0;
float distance5 = 0.0;
float distance6 = 0.0;
float distance7 = 0.0;
float distance8 = 0.0;



enum { TEAM_CHOICE, WAIT, RUN, HOMOLOGATION, END} state; 


void setup() {
  motor.setupMotors();                        // Motor    
  lineFollower.setup();                       // Line follower IR
  Serial.begin(9600);                         // Serial (comm ard-lcd)
  state = TEAM_CHOICE;                        // Statemachine
  pinMode(start_switch_PIN, INPUT_PULLUP);    // Starter_switch
  sonarSensorN.setup(150);                       // Sonar sensor
  sonarSensorW.setup(150);
  sonarSensorE.setup(150);
  sonarSensorS.setup(150);
  sonarSensorNW.setup(150);
  sonarSensorNE.setup(150);
  sonarSensorSW.setup(150);
  sonarSensorSE.setup(150);
}


void loop() {
  //encoderLogic.update();
  
  switch(state){
    case TEAM_CHOICE:
    {
      team = communicationArduinoLCD.chooseTeam();
      if (team != 0){
        state = WAIT;
        strategy.setTeam(team);
      }
      break;
    }
    case WAIT :
    {
      if(digitalRead(start_switch_PIN)==HIGH){
        state = RUN;
      }
      break;
      
    }
    case RUN:
    {
      timeNow = millis()-timeStartRUN;
        if (sonarSensorN.detection() || sonarSensorW.detection() || sonarSensorE.detection() ||sonarSensorS.detection() ||sonarSensorNE.detection() ||sonarSensorNW.detection() ||sonarSensorSE.detection()||sonarSensorSW.detection()) {
          Movement::stopMovement();
          state = HOMOLOGATION;
        }
        else{
          strategy.play();
        }
      break;
    }
    case HOMOLOGATION:
    {
      if (!(sonarSensorN.detection() || sonarSensorW.detection() || sonarSensorE.detection() ||sonarSensorS.detection() ||sonarSensorNE.detection() ||sonarSensorNW.detection() ||sonarSensorSE.detection()||sonarSensorSW.detection())){
        state = RUN;
      }
      break;
    }
    case END:
    {
    /*
      Serial.print("Distance cycle Motor Two: ");                   // Encoder
      Serial.print(encoderLogic.getDistanceMotorTwo());
      Serial.println(" cm");

      Serial.print("Distance cycle Motor Four: ");
      Serial.print(encoderLogic.getDistanceMotorFour());
      Serial.println(" cm");

      Serial.print("Total Distance Motor Two: ");
      Serial.print(encoderLogic.getTotalDistanceMotorTwo());
      Serial.println(" cm");

      Serial.print("Total Distance Motor Four: ");
      Serial.print(encoderLogic.getTotalDistanceMotorFour());
      Serial.println(" cm");

      delay(1000);
      */

      Movement::stopMovement();
      state = TEAM_CHOICE;
      // Serial.println("END");
      break;
    }
  }
  delay(50);
}
