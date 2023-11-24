#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);

#define IRleft_PIN 4  
#define IRright_PIN 5

int detectionLeft = HIGH;    // no obstacle
int detectionRigth = HIGH; 

int vL = 75; //Low speed left
int VL = 125; //Hight speed left
int vR = 75; //Low speed right
int VR = 125; //Hight speed right
int v = 100; //speed forward

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
}

void loop() {
  measureDistance();
  if (distance_mm != 0 && distance_mm < 50.0){
    MoveStop();
  }
  else {
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
  }else if(detectionLeft == HIGH and detectionRigth == LOW){
    LeftMotor->setSpeed(vL);
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(VR);
    RightMotor->run(FORWARD);
  }else if(detectionLeft == LOW and detectionRigth == LOW){
    LeftMotor->setSpeed(v);
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(v);
    RightMotor->run(FORWARD);
  } 
  else{
    LeftMotor->setSpeed(0);
    LeftMotor->run(RELEASE);
    RightMotor->setSpeed(0);
    RightMotor->run(RELEASE);
    endLine = true;
  }
  return endLine;
}
