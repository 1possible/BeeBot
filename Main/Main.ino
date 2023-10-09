#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);

int DIGITAL_OUT_L=2;
int DIGITAL_OUT_R=3;
int stepsL = 0;
int stepsR = 0;
int var = 0;

const byte TRIGGER_PIN = 2; 
const byte ECHO_PIN = 3;    
const unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;

void checkStepStateL(){
  if (digitalRead(DIGITAL_OUT_L) == HIGH){
    stepsL +=1;
  }
}
void checkStepStateR(){
  if (digitalRead(DIGITAL_OUT_R) == HIGH){
    stepsR +=1;
  }
}

void setup() {
  // Set DIGITAL_OUT as an INPUT
  pinMode(DIGITAL_OUT_L, INPUT);
  pinMode(DIGITAL_OUT_R, INPUT);
  
  Serial.begin(9600);

  // attach Interrupt to Interrupt Service Routine
  attachInterrupt(digitalPinToInterrupt(DIGITAL_OUT_L), checkStepStateL, RISING);
  attachInterrupt(digitalPinToInterrupt(DIGITAL_OUT_R), checkStepStateR, RISING);
  
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
  uint8_t i;

  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
  float distance_mm = measure / 2.0 * SOUND_SPEED;

   switch(var){
    case 0:
      Serial.print("case 0");
      LeftMotor->run(FORWARD);
      RightMotor->run(FORWARD);
      for (i=0; i<255; i++) {
        LeftMotor->setSpeed(i);
        RightMotor->setSpeed(i);
        delay(20);
      }
      var = 1;
      break;
    case 1:
      Serial.print("case 1");
      LeftMotor->run(BACKWARD);
      RightMotor->run(FORWARD);
      for (i=0; i<255; i++){
        LeftMotor->setSpeed(75);
        RightMotor->setSpeed(75);
        delay(10);
      }
      var = 0;
      break;
    case 2:
      Serial.print("case 3");
      LeftMotor->run(FORWARD);
      RightMotor->run(BACKWARD);
      for (i=0; i<255; i++){
        LeftMotor->setSpeed(75);
        RightMotor->setSpeed(75);
        delay(10);
      }
      var = 0;
      break;
    case 3:
      Serial.print("case 3");
      LeftMotor->run(BACKWARD);
      RightMotor->run(BACKWARD);
      for (i=0; i<255; i++){
        LeftMotor->setSpeed(75);
        RightMotor->setSpeed(75);
        delay(10);
      }
      var = 0;
      break;
   }

  Serial.print("end");
  LeftMotor->run(RELEASE);
  RightMotor->run(RELEASE);
  delay(1000);
}
