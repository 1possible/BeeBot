//GreatRobot code test-encodeur
#include <Encoder.h>
#include <Adafruit_MotorShield.h>
//motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *MotorTwo = AFMS.getMotor(2);
Encoder encoTwo(10,11);
//Encoder encoFour(12,13);
//Adafruit_DCMotor *MotorFour = AFMS.getMotor(4);

long positionLeft  = -999;
int rot = 1;

enum { WAIT, RUN, END} state; 
//enum {NODEBUG, DEBUG} debug_state;
unsigned long timeStartRUN;
unsigned long timeNow;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  Serial.println("comm esp");
  long newLeft;
  newLeft = encoTwo.read();
  if (newLeft != positionLeft) {
    Serial.print("Left = ");
    Serial.print(newLeft);
    Serial.println();
    positionLeft = newLeft;
  }
  switch(state){
    case WAIT:
    {
      if(Serial.available()){
        String dataReceived = Serial.readString();
        if(dataReceived == "run"){
          state = RUN;
          startRUNNING();
        }
      }
    }
    case RUN:
    {
      timeNow = millis()-timeStartRUN;
      //Serial.print("RUN "+String(timeNow)+" ms\n");
      if(timeNow >= 3000){
        state = END;
      }
      break;
    }
    case END:
    {
      Serial.print("stop motor");
      MotorTwo->run(RELEASE); 
      //MotorFour->run(RELEASE);
      state = WAIT;
      break;
    }
  }
  delay(50);
}

void startRUNNING(){
  state = RUN;
  Serial.print("start running");
  timeStartRUN=millis();
  MotorTwo->setSpeed(250); 
  //MotorFour->setSpeed(250);
  if(rot ==1){
    MotorTwo->run(FORWARD); 
    //MotorFour->run(FORWARD); 
  }else{
    MotorTwo->run(BACKWARD); 
    //MotorFour->run(BACKWARD);
  }
}
