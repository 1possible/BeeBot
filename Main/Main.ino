#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);

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
  
  LeftMotor->setSpeed(150);
  LeftMotor->run(FORWARD);
  LeftMotor->run(RELEASE);
  RightMotor->setSpeed(150);
  RightMotor->run(FORWARD);
  RightMotor->run(RELEASE);

  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  measureDistance();
  if (distance_mm < 10.0){
    MoveStop();
  }
  else {
    MoveForward();
  }
}

void measureDistance ()
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