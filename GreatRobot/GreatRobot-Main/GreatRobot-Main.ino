//GreatRobot code
#include <Wire.h>
#include <Arduino.h>
#include "CommunicationArduinoLCD.h"
#include "Motor.h"
#include "SonarSensor.h"
#include "EncoderLogic.h"
#include "LineFollower.h"
#include "Movement.h"

// DEFINE PINS
const byte TRIGGER_PIN = 7;           // Sonar sensor pins
const byte ECHO_PIN = 6;
const int encoR_PIN = 2;              // Encoder
const int encoL_PIN = 3;
const int IR_left_PIN = 9;            // IR sensor
const int IR_right_PIN = 10;


// DEFINE INSTANCES FOR CLASSES
CommunicationArduinoLCD communicationArduinoLCD;
Motor motor;
SonarSensor sonar(TRIGGER_PIN, ECHO_PIN);
EncoderLogic encoderLogic(encoR_PIN, encoL_PIN);
LineFollower lineFollower = LineFollower(IR_left_PIN, IR_right_PIN, motor);


// DEFINE CONSTANTS
const uint8_t LOW_SPEED = 75;                                 // motor speeds
const uint8_t HIGH_SPEED = 250;
int team = 1;                                                 // Lcd team       // 1 YELLOW Team 2 BLUE
int timeRight = 0;                                            // sonor sensor
int timeDodgeRight = 0;
unsigned long timeNow;
unsigned long timeStartRUN;


enum { WAIT, RUN, DODGERIGHT, DODGELEFT, END} state; 


void setup() {
  motor.setupMotors();      // Motor
  sonar.setup();            // Sonar sensor
  lineFollower.setup();     // Line follower IR
  Serial.begin(9600);       //serial (comm ard-lcd)
  state = WAIT;             //statemachine
}


void loop() {
  encoderLogic.update();
  
  switch(state){
    case WAIT:
    {
      communicationArduinoLCD.chooseTeam();
      break;
    }
    case RUN:
    {
      timeNow = millis()-timeStartRUN;
      float distance_mm = sonar.measureDistance();
      if(timeNow >= 120000){
        state = END;
      }
      else{
        //capteur sonore
        if (distance_mm != 0 && distance_mm < 100.0){
          Movement::stopMovement();
          timeRight = 80;
          timeDodgeRight = 3;
          Serial.println("tourne droite");
          state = DODGERIGHT;
          break;
        }
        if (timeDodgeRight > 0){
          //tour par la droite
          if (timeRight > 1) {
            Serial.println("timeRight");
            Serial.println(timeRight);
            timeRight--;
            break;
          }
          else if (timeRight == 1){
            timeRight--;
            if (timeDodgeRight > 1){
              timeDodgeRight--;
              timeRight = 80;
              state = DODGELEFT;
              break;
            }
            else if (timeDodgeRight == 1){
              timeDodgeRight--;
              timeRight = 80;
              state = DODGERIGHT;
              break;
            }
            break;
          }
        }
        else{                                               // sinon continue de suivre la ligne
          bool endline =lineFollower.followingLine();
          if(endline){
            state = END;
          }
        }
        
      }
      break;
    }
   case DODGERIGHT:
    {
      Movement::dodgeRight();
      // timeDodgeRight += 1;
      state = RUN;
      break;
    }
    case DODGELEFT:
    {
      Movement::dodgeLeft();
      // timeDodgeLeft += 1;
      state = RUN;
      break;
    }
    case END:
    {
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

      Movement::stopMovement();
      state = WAIT;
      // Serial.println("END");
      break;
    }
  }
  delay(50);
}
void startRUN(){
  state = RUN;
  //Serial.println("RUN");
  timeStartRUN = millis();
}