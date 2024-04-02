//GreatRobot code
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "LineFollower.h"

//motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(3);

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

//pin sonor sensor
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN1 = 6;

//IR sensor
const int IR_left_PIN = 9;
const int IR_right_PIN = 10;
LineFollower lineFollower = LineFollower(IR_left_PIN, IR_right_PIN);

//starter switch (cordon)
const int start_switch_PIN = 11;


enum { TEAM_CHOICE, WAIT, RUN, STEP_FORWARD, STEP_BACKWARD, DODGERIGHT, DODGELEFT, END} state; 

unsigned long timeNow;
unsigned long timeStartRUN;
unsigned long timeStartStep;


//lcd
int team = 1; // 1 YELLOW Team 2 BLUE

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
  AFMS.begin();

  //encoder
  pinMode(encoR_PIN, INPUT);
  pinMode(encoL_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), encoderMotorTwoChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), encoderMotorFourChange, CHANGE);

  //serial (comm ard-lcd)
  Serial.begin(9600);
  //Serial.print("setup...");

  //IR MOTOR
  lineFollower.setup();

  //starter_switch
  pinMode(start_switch_PIN, INPUT_PULLUP);

  //sonor
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN1, INPUT);
  
  //statemachine
  state = TEAM_CHOICE;
}

void loop() {
  float distanceMotorTwo = (encoderCountMotorTwo / stepsPerRevolution) * wheelPerimeterCm;
  float distanceMotorFour = (encoderCountMotorFour / stepsPerRevolution) * wheelPerimeterCm;
  // Mettre à jour la distance totale parcourue
  totalDistanceMotorTwo += distanceMotorTwo;
  totalDistanceMotorFour += distanceMotorFour;

  switch(state){
    case TEAM_CHOICE:
    {
  	  String readString;
      String Q;
      //delay(1);
      if(Serial.available() > 0){
        char c = Serial.read();
        readString += c;
      }
      Q = readString;
      //YELLOW TEAM
      if (Q == "1"){
        team = 1;
        state = WAIT;
      }
      //BLUE TEAM
      else if (Q == "2"){
        team = 2;
        state = WAIT;
      }
      break;
    }
    case WAIT :
    {
      if(digitalRead(start_switch_PIN)==HIGH){
        startRUN();
      }
      break;
      
    }
    case RUN:
    {
      timeNow = millis()-timeStartRUN;
      distance_mm1 = measureDistance(TRIGGER_PIN, ECHO_PIN1);
      if(timeNow >= 120000){
        Serial.println("chrono off");
        state = END;
      }
      else{
        if (distance_mm1 != 0 && distance_mm1 < 100.0){
          RightMotor->setSpeed(0); 
          LeftMotor->setSpeed(0);
          RightMotor->run(RELEASE); 
          LeftMotor->run(RELEASE);
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
        else{
          bool endline =lineFollower.followingLine(LeftMotor,RightMotor);
          if(endline){
            timeStartStep= millis();
            state = STEP_FORWARD;
            //Serial.println("stat start forward");
          }
        }
        
      }
      break;
    }
   case STEP_FORWARD:
   {
      if(millis()-timeStartStep > 2000){
        timeStartStep= millis();
        state = STEP_BACKWARD;
        //Serial.println("stat start backward");
      }else{
        RightMotor->setSpeed(225); 
        LeftMotor->setSpeed(225);
        RightMotor->run(FORWARD); 
        LeftMotor->run(FORWARD);
      }
      break;
   }
   case STEP_BACKWARD:
   {
      if(millis()-timeStartStep > 5000){
        timeStartStep= millis();
        //Serial.println("stat start END");
        state = END;
      }else{
        RightMotor->setSpeed(225); 
        LeftMotor->setSpeed(225);
        RightMotor->run(BACKWARD); 
        LeftMotor->run(BACKWARD);
      }
      break;
   }
   case DODGERIGHT:
    {
      //angle droit
      RightMotor->setSpeed(225); 
      LeftMotor->setSpeed(225);
      RightMotor->run(BACKWARD); 
      LeftMotor->run(FORWARD);
      delay(3955);
      RightMotor->setSpeed(225); 
      LeftMotor->setSpeed(225);
      RightMotor->run(FORWARD); 
      LeftMotor->run(FORWARD);
      // timeDodgeRight += 1;
      state = RUN;
      break;
    }
    case DODGELEFT:
    {
      //angle droit
      RightMotor->setSpeed(225); 
      LeftMotor->setSpeed(225);
      RightMotor->run(FORWARD); 
      LeftMotor->run(BACKWARD);
      delay(3800);
      RightMotor->setSpeed(225); 
      LeftMotor->setSpeed(225);
      RightMotor->run(FORWARD); 
      LeftMotor->run(FORWARD);
      
      // timeDodgeLeft += 1;
      state = RUN;
      break;
    }
    case END:
    {
      // Afficher les distances
      //Serial.print("Distance cycle Motor Two: ");
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

      RightMotor->run(RELEASE); 
      LeftMotor->run(RELEASE);
      state = TEAM_CHOICE;
      //Serial.println("END");
      break;
    }
  }
  delay(50);
}
void startRUN(){
  state = RUN;
  //Serial.println("RUN");
  lineFollower.setTeam(team);
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
