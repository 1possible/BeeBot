//GreatRobot code
#include "CommunicationArduinoLCD.h"
#include "Motor.h"
#include "SonarSensor.h"
#include "LineFollower.h"
#include "Movement.h"
#include "Strategy.h"
#include "DetectionManager.h"
#include <ArduinoLog.h>

// DEFINE PINS
//const int encoR_PIN = 2;              // Encoder
//const int encoL_PIN = 3;
const int IR_left_PIN = 9;            // IR sensor
const int IR_right_PIN = 10;
const int TRIGGER_PIN = 52;           // Pin sonor sensor
const int ECHO_PIN_S = 44; 
const int ECHO_PIN_SW = 45;  
const int ECHO_PIN_SE = 46; 
const int ECHO_PIN_W = 47; 
const int ECHO_PIN_E = 48; 
const int ECHO_PIN_NW = 49; 
const int ECHO_PIN_NE = 50; 
const int ECHO_PIN_N = 51; 
const int start_switch_PIN = 11;      // Starter switch (cordon)


// DEFINE INSTANCES FOR CLASSES
CommunicationArduinoLCD communicationArduinoLCD;
Motor motor;
//EncoderLogic encoderLogic(encoR_PIN, encoL_PIN);
LineFollower lineFollower = LineFollower(IR_left_PIN, IR_right_PIN);
Strategy strategy = Strategy(&lineFollower);
SonarSensor sonarSensorN(TRIGGER_PIN, ECHO_PIN_N);
SonarSensor sonarSensorW(TRIGGER_PIN, ECHO_PIN_W);
SonarSensor sonarSensorE(TRIGGER_PIN, ECHO_PIN_E);
SonarSensor sonarSensorS(TRIGGER_PIN, ECHO_PIN_S);
SonarSensor sonarSensorNW(TRIGGER_PIN, ECHO_PIN_NW);
SonarSensor sonarSensorNE(TRIGGER_PIN, ECHO_PIN_NE);
SonarSensor sonarSensorSW(TRIGGER_PIN, ECHO_PIN_SW);
SonarSensor sonarSensorSE(TRIGGER_PIN, ECHO_PIN_SE);
DetectionManager detectionManager(&sonarSensorN,&sonarSensorNE,&sonarSensorE,&sonarSensorSE,&sonarSensorS,&sonarSensorSW,&sonarSensorW,&sonarSensorNW);

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

unsigned long timeLastDebug;
unsigned long timeToUpdateDebug = 1000;

enum { TEAM_CHOICE, WAIT, RUN, DETECTION, END} state; 


void setup() {
  Log.begin(LOG_LEVEL_SILENT, &Serial,true);
  /*
  *** LOG level ***
  * 0 - LOG_LEVEL_SILENT     no output 
  * 1 - LOG_LEVEL_FATAL      fatal errors 
  * 2 - LOG_LEVEL_ERROR      all errors  
  * 3 - LOG_LEVEL_WARNING    errors, and warnings 
  * 4 - LOG_LEVEL_NOTICE     errors, warnings and notices 
  * 5 - LOG_LEVEL_TRACE      errors, warnings, notices & traces 
  * 6 - LOG_LEVEL_VERBOSE    all 
  */

  //Start logging

  Log.notice("******************************************" CR);                     
  Log.notice("***        Logging: MAIN arduino        ***" CR);                      
  Log.notice("******************************************" CR);

  motor.setupMotors();                        // Motor    
  Movement::setup();
  lineFollower.setup();                       // Line follower IR
  Serial.begin(9600);                         // Serial (comm ard-lcd)
  state = TEAM_CHOICE;                        // Statemachine
  pinMode(start_switch_PIN, INPUT_PULLUP);    // Starter_switch
  detectionManager.setup();
  timeLastDebug = millis();
  strategy.setup();

  Log.trace("MAIN : finished setup");
}


void loop() {
  //encoderLogic.update();
  
  switch(state){
    case TEAM_CHOICE:
    {
      team = communicationArduinoLCD.chooseTeam();
      if (team != 0){
        Log.notice("MAIN : new state : WAIT");
        state = WAIT;
        strategy.setTeam(team);
      }
      break;
    }

    case WAIT :
    {
      if(digitalRead(start_switch_PIN)==HIGH){
        Log.notice("MAIN : new state : RUN");
        state = RUN;
      }
      break;
      
    }
    
    case RUN:
    {
      timeNow = millis()-timeStartRUN;
        if (detectionManager.detection()) {
          Log.notice("MAIN : new state : DETECTION");
          Movement::stopMovement();
          strategy.disableTimer();
          state = DETECTION;
        }
        else{
          strategy.play();
        }
        /*
        if(millis()-timeLastDebug >= timeToUpdateDebug){
            Serial.println("---RUN---");
            Serial.print("Distance N: ");
            Serial.println(sonarSensorN.getDistance());
            Serial.print("Distance E: ");
            Serial.println(sonarSensorE.getDistance());
            Serial.print("Distance W: ");
            Serial.println(sonarSensorW.getDistance());
            Serial.print("Distance S: ");
            Serial.println(sonarSensorS.getDistance());
            Serial.print("Distance NE: ");
            Serial.println(sonarSensorNE.getDistance());
            Serial.print("Distance NW: ");
            Serial.println(sonarSensorNW.getDistance());
            Serial.print("Distance SE: ");
            Serial.println(sonarSensorSE.getDistance());
            Serial.print("Distance SW: ");
            Serial.println(sonarSensorSW.getDistance());
            timeLastDebug = millis();
          }*/
      break;
    }
    case DETECTION:
    {
      if (!detectionManager.detection()){
        Log.notice("MAIN : new state : RUN");
        state = RUN;
        strategy.activateTimer();
      }
      /*
      if(millis()-timeLastDebug >= timeToUpdateDebug){
        Serial.println("--STOP--");
          Serial.print("Distance N: ");
          Serial.println(sonarSensorN.getDistance());
          Serial.print("Distance E: ");
          Serial.println(sonarSensorE.getDistance());
          Serial.print("Distance W: ");
          Serial.println(sonarSensorW.getDistance());
          Serial.print("Distance S: ");
          Serial.println(sonarSensorS.getDistance());
          Serial.print("Distance NE: ");
          Serial.println(sonarSensorNE.getDistance());
          Serial.print("Distance NW: ");
          Serial.println(sonarSensorNW.getDistance());
          Serial.print("Distance SE: ");
          Serial.println(sonarSensorSE.getDistance());
          Serial.print("Distance SW: ");
          Serial.println(sonarSensorSW.getDistance());
          timeLastDebug = millis();
          }
          */
      break;
    }

    case END:
    {
      Log.notice("MAIN : new state : TEAM_CHOICE");
      Movement::stopMovement();
      state = TEAM_CHOICE;
      break;
    }
  }
  delay(50);
}
