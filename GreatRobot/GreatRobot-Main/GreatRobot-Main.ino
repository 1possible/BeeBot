//GreatRobot code
#include <Wire.h>
#include "CommunicationArduinoLCD.h"
#include "Motor.h"

// Create an instances of class
CommunicationArduinoLCD communicationArduinoLCD;
Motor motor;


// Define constants for motor speeds
const uint8_t LOW_SPEED = 75;
const uint8_t HIGH_SPEED = 250;

// Define constant for lcd team
int team = 1; // 1 YELLOW Team 2 BLUE




// Paramètres de l'encodeur et de la roue
const float stepsPerRevolution = 360; // Nombre de pas par tour de l'encodeur
const float wheelDiameterCm = 4.5; // Diamètre de la roue en cm
const float wheelPerimeterCm = 3.14159 * wheelDiameterCm; // Périmètre de la roue

// Variables pour suivre la distance totale parcourue
float totalDistanceMotorTwo = 0.0;
float totalDistanceMotorFour = 0.0;

int encoR_PIN = 2;
int encoL_PIN = 3;
volatile long encoderCountMotorTwo = 0;
volatile long encoderCountMotorFour = 0;

// Capteur sonore
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN1 = 6;

//IR sensor
#define IR_left_PIN 9
#define IR_right_PIN 10

enum { WAIT, RUN, DODGERIGHT, DODGELEFT, END} state; 

unsigned long timeNow;
unsigned long timeStartRUN;
unsigned long timeLine = 0;


//// Capteur sonore

//sonor sensor constant
float distance_mm1 = 0.0;
const unsigned long MEASURE_TIMEOUT = 22000UL;
const float SOUND_SPEED = 340.0 / 1000;
int timeRight = 0;
int timeDodgeRight = 0;


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
  //motor
  motor.setupMotors();

  //encoder
  pinMode(encoR_PIN, INPUT);
  pinMode(encoL_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), encoderMotorTwoChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), encoderMotorFourChange, CHANGE);

  //serial (comm ard-lcd)
  Serial.begin(9600);
  //Serial.print("setup...");

  //IR MOTOR
  pinMode(IR_left_PIN, INPUT);
  pinMode(IR_right_PIN, INPUT);

  // Capteur sonore
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN1, INPUT);
  
  //statemachine
  state = WAIT;
}

void loop() {
  float distanceMotorTwo = (encoderCountMotorTwo / stepsPerRevolution) * wheelPerimeterCm;
  float distanceMotorFour = (encoderCountMotorFour / stepsPerRevolution) * wheelPerimeterCm;
  // Mettre à jour la distance totale parcourue
  totalDistanceMotorTwo += distanceMotorTwo;
  totalDistanceMotorFour += distanceMotorFour;

  switch(state){
    case WAIT:
    {
      communicationArduinoLCD.chooseTeam();
      break;
    }
    case RUN:
    {
      timeNow = millis()-timeStartRUN;
      distance_mm1 = measureDistance(TRIGGER_PIN, ECHO_PIN1); // capteur sonore appel
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
      // Afficher les distances
      Serial.print("Distance cycle Motor Two: ");
      Serial.print(distanceMotorTwo);
      Serial.println(" cm");
      Serial.print("Distance cycle Motor Four: ");
      Serial.print(distanceMotorFour);
      Serial.println(" cm");
    
      Serial.print("Total Distance Motor Two: ");
      Serial.print(totalDistanceMotorTwo);
      Serial.println(" cm");
      Serial.print("Total Distance Motor Four: ");
      Serial.print(totalDistanceMotorFour);
      Serial.println(" cm");

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

// Fonctions d'interruption pour les encodeurs
void encoderMotorTwoChange() {
  if (digitalRead(2) == HIGH)
    encoderCountMotorTwo++;
  else
    encoderCountMotorTwo--;
}
void encoderMotorFourChange() {
  if (digitalRead(3) == HIGH)
    encoderCountMotorFour++;
  else
    encoderCountMotorFour--;
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
