
#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(4);

#define IRleft_PIN 4  
#define IRright_PIN 5

int vL = 75;
int VL = 125;
int vR = 75;
int VR = 125;
int v = 100;

int detectionLeft = HIGH;    // no obstacle
int detectionRigth = HIGH; 
void setup() {
  Serial.begin(9600);   
  pinMode(IRleft_PIN, INPUT); 
  pinMode(IRright_PIN, INPUT); 

  if (!AFMS.begin()) {         
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
}
void loop() {  
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
  }
}
