//GreatRobot code
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "LineFollower.h"
#include "Strategy.h"

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
const byte TRIGGER_PIN = 8; 
const byte ECHO_PIN1 = 7;
const byte ECHO_PIN2 = 6;  
const byte ECHO_PIN3 = 5;
const byte ECHO_PIN4 = 4;  

//IR sensor
const int IR_left_PIN = 9;
const int IR_right_PIN = 10;
LineFollower lineFollower = LineFollower(IR_left_PIN, IR_right_PIN);

//starter switch (cordon)
const int start_switch_PIN = 11;


enum { TEAM_CHOICE, WAIT, RUN, HOMOLOGATION, END} state; 

unsigned long timeNow;
unsigned long timeStartRUN;
unsigned long timeStartStep;


//lcd
int team = 1; // 1 YELLOW Team 2 BLUE

//sonor sensor constant
float distance_mm1 = 0.0;
float distance_mm2 = 0.0;
float distance_mm3 = 0.0;
float distance_mm4 = 0.0;
const unsigned long MEASURE_TIMEOUT = 22000UL;
const float SOUND_SPEED = 340.0 / 1000;
int timeRight = 0;
int timeDodgeRight = 0;

//Strategy
Strategy strategy = Strategy(&lineFollower);

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
  /* Initialise les broches */
  pinMode(ECHO_PIN2, INPUT);
  /* Initialise les broches */
  pinMode(ECHO_PIN3, INPUT);
  /* Initialise les broches */
  pinMode(ECHO_PIN4, INPUT);
  
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
      distance_mm2 = measureDistance(TRIGGER_PIN, ECHO_PIN2);
      distance_mm3 = measureDistance(TRIGGER_PIN, ECHO_PIN3);
      distance_mm4 = measureDistance(TRIGGER_PIN, ECHO_PIN4);
        if (distance_mm1 != 0 && distance_mm1 < 100.0){
          RightMotor->setSpeed(0); 
          LeftMotor->setSpeed(0);
          RightMotor->run(RELEASE); 
          LeftMotor->run(RELEASE);
          state = HOMOLOGATION;
          break;
        }
        if (distance_mm2 != 0 && distance_mm2 < 100.0){
          RightMotor->setSpeed(0); 
          LeftMotor->setSpeed(0);
          RightMotor->run(RELEASE); 
          LeftMotor->run(RELEASE);
          state = HOMOLOGATION;
          break;
        }
        if (distance_mm3 != 0 && distance_mm3 < 100.0){
          RightMotor->setSpeed(0); 
          LeftMotor->setSpeed(0);
          RightMotor->run(RELEASE); 
          LeftMotor->run(RELEASE);
          state = HOMOLOGATION;
          break;
        }
        if (distance_mm4 != 0 && distance_mm4 < 100.0){
          RightMotor->setSpeed(0); 
          LeftMotor->setSpeed(0);
          RightMotor->run(RELEASE); 
          LeftMotor->run(RELEASE);
          state = HOMOLOGATION;
          break;
        }
        
        else{
          strategy.play(LeftMotor,RightMotor);
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
  strategy.setTeam(team);
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
