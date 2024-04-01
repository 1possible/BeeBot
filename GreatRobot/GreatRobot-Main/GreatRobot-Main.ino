//GreatRobot code
#include <Wire.h>
#include <Arduino.h>
#include "CommunicationArduinoLCD.h"
#include "Motor.h"
#include "SonarSensor.h"
#include "EncoderLogic.h"

// DEFINE PINS
// Sonar sensor pins
const byte TRIGGER_PIN = 7;
const byte ECHO_PIN = 6;
// Encoder
const int encoR_PIN = 2;
const int encoL_PIN = 3;

// Creating instances for classes
CommunicationArduinoLCD communicationArduinoLCD;
Motor motor;
SonarSensor sonar(TRIGGER_PIN, ECHO_PIN);
EncoderLogic encoderLogic(encoR_PIN, encoL_PIN);

// DEFINE CONSTANTS
// motor speeds
const uint8_t LOW_SPEED = 75;
const uint8_t HIGH_SPEED = 250;
// Lcd team
int team = 1; // 1 YELLOW Team 2 BLUE
// sonor sensor
float distance_mm1 = 0.0; // pre declare variables for sensor captor (je pense pas que c'est nécessaire mais au cas zou)




//IR sensor
#define IR_left_PIN 9
#define IR_right_PIN 10

enum { WAIT, RUN, DODGERIGHT, DODGELEFT, END} state; 

unsigned long timeNow;
unsigned long timeStartRUN;
unsigned long timeLine = 0;


//// Capteur sonore
int timeRight = 0;
int timeDodgeRight = 0;


void setup() {
  // motor
  motor.setupMotors();

  // sonar sensor
  sonar.setup();

  //serial (comm ard-lcd)
  Serial.begin(9600);
  //Serial.print("setup...");

  //IR MOTOR
  pinMode(IR_left_PIN, INPUT);
  pinMode(IR_right_PIN, INPUT);

  //statemachine
  state = WAIT;
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
      float distance_mm1 = sonar.measureDistance();
      if(timeNow >= 120000){
        state = END;
      }
      else{
        //capteur sonore
        if (distance_mm1 != 0 && distance_mm1 < 100.0){
          motor.controlMotors(0, 0, RELEASE, RELEASE);
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
        } // Capteur sonore fin
        else{ // sinon continue de suivre la ligne
          bool endline =followingLine();
          if(endline){
            state = END;
          }
        }
        
      }
      break;
    }
   case DODGERIGHT:
    {
      //angle droit
      motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, BACKWARD);
      delay(3955);
      motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, FORWARD);
      // timeDodgeRight += 1;
      state = RUN;
      break;
    }
    case DODGELEFT:
    {
      //angle droit
      motor.controlMotors(HIGH_SPEED, HIGH_SPEED, BACKWARD, FORWARD);
      delay(3800);
      motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, FORWARD);
      
      // timeDodgeLeft += 1;
      state = RUN;
      break;
    }
    case END:
    {
      Serial.print("Distance cycle Motor Two: ");
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

      delay(1000); // Adjust delay as needed

      motor.stopMotors();
      state = WAIT;
      //Serial.println("END");
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


bool followingLine(){
  bool endLine = false;
  bool detectionLeft = digitalRead(IR_left_PIN);
  bool detectionRigth = digitalRead(IR_right_PIN);
  if(detectionLeft == LOW and detectionRigth == HIGH){
    motor.controlMotors(HIGH_SPEED, LOW_SPEED, FORWARD, BACKWARD);
    timeLine = millis();
  }else if(detectionLeft == HIGH and detectionRigth == LOW){
    motor.controlMotors(LOW_SPEED, HIGH_SPEED, BACKWARD, FORWARD);
    timeLine = millis();
  }else if(detectionLeft == LOW and detectionRigth == LOW){
    motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, FORWARD);
    timeLine =millis();
  }else if(detectionLeft == HIGH and detectionRigth == HIGH and (millis()-timeLine) >250){
    motor.stopMotors();
    endLine = true;
  }
  else{//HIGH HIGH without time out
    motor.controlMotors(HIGH_SPEED, HIGH_SPEED, FORWARD, FORWARD);
  }
  return endLine;
}