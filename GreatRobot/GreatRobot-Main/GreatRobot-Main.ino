//GreatRobot code
#include <Wire.h>
#include <Adafruit_MotorShield.h>
//motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(4);
int vL = 75; //Low speed left //75
int VL = 250; //Hight speed left
int vR = 75; //Low speed right //50
int VR = 250; //Hight speed right
int v = 250; //speed forward 

//IR sensor
#define IR_left_PIN 9
#define IR_right_PIN 10

//sensor distance
/*float distance_mm = 0.0;
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN = 6;   
const unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;
*/
enum { WAIT, RUN, END} state; 

unsigned long timeNow;

unsigned long timeStartRUN;
unsigned long timeLine = 0;

//lcd
int team = 1; // 1 YELLOW Team 2 BLUE


void setup() {
  //motor
  AFMS.begin();
  //serial (comm ard-lcd)
  Serial.begin(9600);

  //IR MOTOR
  pinMode(IR_left_PIN, INPUT);
  pinMode(IR_right_PIN, INPUT);
  
  //test US sensor
  /*pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);*/

  //statemachine
  state = WAIT;
}

void loop() {
    
  /*measureDistance();
  Serial.print(distance_mm);
  Serial.println(" mm");*/


  switch(state){
    case WAIT:
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
        startRUN();
      }
      //BLUE TEAM
      else if (Q == "2"){
        team = 2;
        startRUN();
      }
      break;
    }
    case RUN:
    {
      timeNow = millis()-timeStartRUN;
      if(timeNow >= 3000){
        state = END;
      }
      bool endline =followingLine();
      if(endline){
        state = END;
      }
      break;
    }
    case END:
    {
      RightMotor->run(RELEASE); 
      LeftMotor->run(RELEASE);
      state = WAIT;
      break;
    }
  }
  delay(50);
}
void startRUN(){
  state = RUN;
  timeStartRUN = millis();
}

bool followingLine(){
  bool endLine = false;
  bool detectionLeft = digitalRead(IR_left_PIN);
  bool detectionRigth = digitalRead(IR_right_PIN);
  if(detectionLeft == LOW and detectionRigth == HIGH){
    LeftMotor->setSpeed(VL);
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(vR);
    RightMotor->run(BACKWARD);
    timeLine = millis();
  }else if(detectionLeft == HIGH and detectionRigth == LOW){
    LeftMotor->setSpeed(vL);
    LeftMotor->run(BACKWARD );
    RightMotor->setSpeed(VR);
    RightMotor->run(FORWARD);
    timeLine = millis();;
  }else if(detectionLeft == LOW and detectionRigth == LOW){
    LeftMotor->setSpeed(v);
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
  else{//HIGH HIGH without time out
    LeftMotor->setSpeed(v); 
    LeftMotor->run(FORWARD);
    RightMotor->setSpeed(v);
    RightMotor->run(FORWARD);
  }
  return endLine;
}
/*int measureDistance ()
{
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  unsigned long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
  distance_mm = measure / 2.0 * SOUND_SPEED;
  return distance_mm ; 
}*/
