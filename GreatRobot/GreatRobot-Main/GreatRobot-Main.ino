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
const int encoR_PIN = 2;              // Encoder
const int encoL_PIN = 3;
const int IR_left_PIN = 9;            // IR sensor
const int IR_right_PIN = 10;
//pin sonor sensor
const byte TRIGGER_PIN = 8; 
const byte ECHO_PIN1 = 7;
const byte ECHO_PIN2 = 6;  
const byte ECHO_PIN3 = 5;
const byte ECHO_PIN4 = 4;  


// DEFINE INSTANCES FOR CLASSES
CommunicationArduinoLCD communicationArduinoLCD;
Motor motor;
SonarSensor sonar(TRIGGER_PIN, ECHO_PIN1);
EncoderLogic encoderLogic(encoR_PIN, encoL_PIN);
LineFollower lineFollower = LineFollower(IR_left_PIN, IR_right_PIN, motor);
//Strategy
Strategy strategy = Strategy(&lineFollower);



// DEFINE CONSTANTS
const uint8_t LOW_SPEED = 75;                                 // motor speeds
const uint8_t HIGH_SPEED = 250;
int team = 1;                                                 // Lcd team       // 1 YELLOW Team 2 BLUE
int timeRight = 0;                                            // sonor sensor
int timeDodgeRight = 0;
unsigned long timeNow;
unsigned long timeStartRUN;

//sonor sensor constant
float distance_mm1 = 0.0;
float distance_mm2 = 0.0;
float distance_mm3 = 0.0;
float distance_mm4 = 0.0;
const unsigned long MEASURE_TIMEOUT = 22000UL;
const float SOUND_SPEED = 340.0 / 1000;

//starter switch (cordon)
const int start_switch_PIN = 11;


enum { TEAM_CHOICE, WAIT, RUN, HOMOLOGATION, END} state; 

int measureDistance (const byte x, const byte y)
{
  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(x, HIGH);
  delayMicroseconds(10);
  digitalWrite(x, LOW);
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  unsigned long measure = pulseIn(y, HIGH, MEASURE_TIMEOUT);
  /* 3. Calcul la distance à partir du temps mesuré */
  int test = measure / 2.0 * SOUND_SPEED; 
  return test;
}


void setup() {
  motor.setupMotors();      // Motor
  sonar.setup();            // Sonar sensor
  lineFollower.setup();     // Line follower IR
  Serial.begin(9600);       //serial (comm ard-lcd)
  state = TEAM_CHOICE;             //statemachine


  //starter_switch
  pinMode(start_switch_PIN, INPUT_PULLUP);
  /*
  //sonor
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN1, INPUT);
  // Initialise les broches 
  pinMode(ECHO_PIN2, INPUT);
  // Initialise les broches 
  pinMode(ECHO_PIN3, INPUT);
  // Initialise les broches 
  pinMode(ECHO_PIN4, INPUT);
  */
}


void loop() {
  encoderLogic.update();
  
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
      //float distance_mm = sonar.measureDistance();
      distance_mm1 = measureDistance(TRIGGER_PIN, ECHO_PIN1);
      distance_mm2 = measureDistance(TRIGGER_PIN, ECHO_PIN2);
      distance_mm3 = measureDistance(TRIGGER_PIN, ECHO_PIN3);
      distance_mm4 = measureDistance(TRIGGER_PIN, ECHO_PIN4);
        if (distance_mm1 != 0 && distance_mm1 < 100.0){
          Movement::stopMovement();
          state = HOMOLOGATION;
        }
        else if (distance_mm2 != 0 && distance_mm2 < 100.0){
          Movement::stopMovement();
          state = HOMOLOGATION;
          break;
        }
        else if (distance_mm3 != 0 && distance_mm3 < 100.0){
          Movement::stopMovement();
          state = HOMOLOGATION;
        }
        else if (distance_mm4 != 0 && distance_mm4 < 100.0){
          Movement::stopMovement();
          state = HOMOLOGATION;
        }else{
          strategy.play();
        }
      break;
    }
    case HOMOLOGATION:
    {
      distance_mm1 = measureDistance(TRIGGER_PIN, ECHO_PIN1);
      distance_mm2 = measureDistance(TRIGGER_PIN, ECHO_PIN2);
      distance_mm3 = measureDistance(TRIGGER_PIN, ECHO_PIN3);
      distance_mm4 = measureDistance(TRIGGER_PIN, ECHO_PIN4);
      if ((distance_mm1 == 0 || distance_mm1 > 100) && (distance_mm2 == 0 || distance_mm2 > 100) && (distance_mm3 == 0 || distance_mm3 > 100) && (distance_mm4 == 0 || distance_mm4 > 100)){
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
