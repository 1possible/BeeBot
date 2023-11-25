#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);

#define switchGroup 8

#define IRleft_PIN 4  
#define IRright_PIN 5

int detectionLeft = HIGH;    // no obstacle
int detectionRigth = HIGH; 

//the true value will be define later in the code in function of switchgroup
int vL = 0; //Low speed left //60
int VL = 0; //Hight speed left //125
int vR = 0; //Low speed right //75
int VR = 0; //Hight speed right //140
int v = 0; //speed forward //90

unsigned long timeLine;

float distance_mm = 0.0;
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN = 6;    
const unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;

bool GoesForward = false;

void setup() {
  Serial.begin(9600);

  if (!AFMS.begin()) {         
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  
  LeftMotor->setSpeed(0);
  LeftMotor->run(RELEASE);
  RightMotor->setSpeed(0);
  RightMotor->run(RELEASE);

  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);

  //pin IRsensor
  pinMode(IRleft_PIN, INPUT); 
  pinMode(IRright_PIN, INPUT); 
  pinMode(switchGroup, INPUT);

  if(digitalRead(switchGroup) == HIGH){
    Serial.println("Blue group");
    vL = 60; //Low speed left
    VL = 125; //Hight speed left 
    vR = 75; //Low speed right 
    VR = 140; //Hight speed right 
    v = 90; //speed forward 
  }else{
    Serial.println("yellow group");
    vL = 55; //Low speed left 
    VL = 115; //Hight speed left
    vR = 75; //Low speed right 
    VR = 200; //Hight speed right
    v = 90; //speed forward 
  }

 
}

void loop() {
  measureDistance();
  if (distance_mm != 0 && distance_mm < 100.0){
    MoveStop();
  }
  else{
    followingLine();
  }
  
}

int measureDistance ()
{
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  unsigned long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
  distance_mm = measure / 2.0 * SOUND_SPEED;
  return distance_mm ; 
}

void MoveStop(){
  LeftMotor->run(RELEASE);
  RightMotor->run(RELEASE);
}

void MoveForward(){
  LeftMotor->run(FORWARD);
  RightMotor->run(FORWARD);
}

void MoveBackward(){
  LeftMotor->run(BACKWARD);
  RightMotor->run(BACKWARD);
}

void turnLeft() {
  LeftMotor->run(BACKWARD);
  RightMotor->run(FORWARD);
}

void turnRight(){
  LeftMotor->run(FORWARD);
  RightMotor->run(BACKWARD);
}
bool followingLine(){
  bool endLine = false;
  detectionLeft = digitalRead(IRleft_PIN);
  detectionRigth = digitalRead(IRright_PIN);
  if(detectionLeft == LOW and detectionRigth == HIGH){
    LeftMotor->setSpeed(VL);
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(vR);
    RightMotor->run(FORWARD);
    timeLine = millis();
  }else if(detectionLeft == HIGH and detectionRigth == LOW){
    LeftMotor->setSpeed(vL-10);
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(VR);
    RightMotor->run(FORWARD);
    timeLine = millis();;
  }else if(detectionLeft == LOW and detectionRigth == LOW){
    LeftMotor->setSpeed(v-10);
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(v);
    RightMotor->run(FORWARD);
    timeLine =millis();
  }else if(detectionLeft == HIGH and detectionRigth == HIGH and (millis()-timeLine) >250){
    LeftMotor->setSpeed(0);
    LeftMotor->run(RELEASE);
    RightMotor->setSpeed(0);
    RightMotor->run(RELEASE);
    endLine = true;
  }
  else{
    LeftMotor->setSpeed(v-50);
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(v);
    RightMotor->run(FORWARD);
    
  }
  return endLine;
}
