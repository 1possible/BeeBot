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
const int ECHO_PIN_1 = 44;
const int ECHO_PIN_2 = 45;  
const int ECHO_PIN_3 = 46;
const int ECHO_PIN_4 = 47; 
const int ECHO_PIN_5 = 48; 
const int ECHO_PIN_6 = 49; 
const int ECHO_PIN_7 = 50; 
const int ECHO_PIN_8 = 51; 
const int ECHO_PIN_SolarPan = 43;            // Pin du capteur ultrason qui détecte les panneaux solaires
const int start_switch_PIN = 11;      // Starter switch (cordon)


// DEFINE INSTANCES FOR CLASSES
CommunicationArduinoLCD communicationArduinoLCD;
Motor motor;
//EncoderLogic encoderLogic(encoR_PIN, encoL_PIN);
LineFollower lineFollower = LineFollower(IR_left_PIN, IR_right_PIN);
SonarSensor sonarSensor1(TRIGGER_PIN, ECHO_PIN_1);
SonarSensor sonarSensor2(TRIGGER_PIN, ECHO_PIN_2);
SonarSensor sonarSensor3(TRIGGER_PIN, ECHO_PIN_3);
SonarSensor sonarSensor4(TRIGGER_PIN, ECHO_PIN_4);
SonarSensor sonarSensor5(TRIGGER_PIN, ECHO_PIN_5);
SonarSensor sonarSensor6(TRIGGER_PIN, ECHO_PIN_6);
SonarSensor sonarSensor7(TRIGGER_PIN, ECHO_PIN_7);
SonarSensor sonarSensor8(TRIGGER_PIN, ECHO_PIN_8);
SonarSensor sonarSensorSolarPan(TRIGGER_PIN, ECHO_PIN_SolarPan);
SolarPanels solarPanel = SolarPanels(&sonarSensorSolarPan);
Strategy strategy = Strategy(&lineFollower, &solarPanel );


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
  sonarSensor1.setup();                       // Sonar sensor
  sonarSensor2.setup();
  sonarSensor3.setup();
  sonarSensor4.setup();
  sonarSensor5.setup();
  sonarSensor6.setup();
  sonarSensor7.setup();
  sonarSensor8.setup();
  sonarSensorSolarPan.setup();
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
      distance1 = sonarSensor1.measureDistance();
      distance2 = sonarSensor2.measureDistance();
      distance3 = sonarSensor3.measureDistance();
      distance4 = sonarSensor4.measureDistance();
      distance5 = sonarSensor5.measureDistance();
      distance6 = sonarSensor6.measureDistance();
      distance7 = sonarSensor7.measureDistance();
      distance8 = sonarSensor8.measureDistance();
      /*int distanceTest = sonarSensorSolarPan.measureDistance();
      Serial.println("distance 1 : "+String(distance1));
      Serial.println("distance 2 : "+String(distance2));
      Serial.println("distance 7 : "+String(distance7));
      Serial.println("distance 8 : "+String(distance8));
      Serial.println("distance test : "+String(distanceTest));*/
        if ((distance1 != 0 && distance1 < 100.0) || (distance2 != 0 && distance2 < 100.0)|| (distance3 != 0 && distance3 < 100.0) ||(distance4 != 0 && distance4 < 100.0) ||(distance5 != 0 && distance5 < 100.0) ||(distance6 != 0 && distance6 < 100.0) ||(distance7 != 0 && distance7 < 100.0) ||(distance8 != 0 && distance8 < 100.0)) {
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
      distance1 = sonarSensor1.measureDistance();
      distance2 = sonarSensor2.measureDistance();
      distance3 = sonarSensor3.measureDistance();
      distance4 = sonarSensor4.measureDistance();
      distance5 = sonarSensor5.measureDistance();
      distance6 = sonarSensor6.measureDistance();
      distance7 = sonarSensor7.measureDistance();
      distance8 = sonarSensor8.measureDistance();
      if ((distance1 == 0 || distance1 > 100) && (distance2 == 0 || distance2 > 100) && (distance3 == 0 || distance3 > 100) && (distance4 == 0 || distance4 > 100)&& (distance5 == 0 || distance5 > 100)&& (distance6 == 0 || distance6 > 100)&& (distance7 == 0 || distance7 > 100)&& (distance8 == 0 || distance8 > 100)){
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
