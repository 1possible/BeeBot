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
enum { WAIT, RUN,COMMAND, END} state; 
enum { NO,UP, DOWN, ROTLEFT, ROTRIGHT} command;
enum {NODEBUG, DEBUG} debug_state;
int timeMsgDEBUG = 1000;
unsigned long timeStartRUN;
unsigned long timeStartCOMMAND;
unsigned long timeStartDEBUG=0;
unsigned long timeNow;
unsigned long timeLine = 0;

//lcd
int team = 1; // 1 YELLOW Team 2 BLUE

//esp BLE
#define EN_BLE_PIN 8 
const int ESP_addrI2C = 3;
String messageBLE = "";

void setup() {
  // put your setup code here, to run once:
  //BLE
  pinMode(EN_BLE_PIN,INPUT);
  Wire.begin();
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
  debug_state = NODEBUG;
}

void loop() {
  bool en_BLE = digitalRead(EN_BLE_PIN);
  messageBLE ="";
  if(en_BLE == HIGH){
    Serial.println("comm esp");
    Wire.requestFrom(ESP_addrI2C,6);
    while(Wire.available()){
      char c = Wire.read();
      messageBLE+= c;
    }
    msgInstru(messageBLE);
  }
    
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
        //state = WAIT;
        printBLE("YELLOW Team");
      }
      //BLUE TEAM
      else if (Q == "2"){
        team = 2;
        //state = WAIT;
        printBLE("Blue Team");
      }
      break;
    }
    case RUN:
    {
      //printBLE("RUN cc");
      /*timeNow = millis()-timeStartRUN;
      //printBLE("RUN "+String(timeNow)+" ms\n");
      if(timeNow >= 3000){
        state = END;
      }*/
      bool endline =followingLine();
      if(endline){
        state = END;
      }
      break;
    }
    case END:
    {
      printBLE("stop motor");
      RightMotor->run(RELEASE); 
      LeftMotor->run(RELEASE);
      state = WAIT;
      break;
    }
    case COMMAND:
    {
      if(command != NO){
        timeNow = millis()-timeStartCOMMAND;
        //printBLE("RUN "+String(timeNow)+" ms\n");
        if(timeNow >= 3000){
          printBLE("time out");
          RightMotor->run(RELEASE); 
          LeftMotor->run(RELEASE);
          command = NO;
        }
      }
      switch(command){
        case UP:
        {
          LeftMotor->setSpeed(250);
          LeftMotor->run(FORWARD);
          RightMotor->setSpeed(250);
          RightMotor->run(FORWARD);
          break;
        }
        case DOWN:
        {
          LeftMotor->setSpeed(250);
          LeftMotor->run(BACKWARD);
          RightMotor->setSpeed(250);
          RightMotor->run(BACKWARD);
          break;
        }
        case ROTLEFT:
        {
          LeftMotor->setSpeed(250);
          LeftMotor->run(BACKWARD);
          RightMotor->setSpeed(250);
          RightMotor->run(FORWARD);
          break;
        }
        case ROTRIGHT:
        {
          LeftMotor->setSpeed(250);
          LeftMotor->run(FORWARD);
          RightMotor->setSpeed(250);
          RightMotor->run(BACKWARD);
          break;
        }
      }
      break;
    }
  }
  switch(debug_state){
    case DEBUG:
    {
      timeNow = millis() - timeStartDEBUG;
      if(timeNow > timeMsgDEBUG){
        timeStartDEBUG = millis();
        printBLE("leftIR : "+String(digitalRead(IR_left_PIN))+ "\n");
        printBLE("rightIR : "+String(digitalRead(IR_right_PIN))+ "\n");
        /*
        measureDistance();
        printBLE(String(distance_mm)+ " mm\n");
        */
      }
    }
  }
  delay(50);
}

void startRUNNING(){
	state = RUN;
  printBLE("start running");
  timeStartRUN=millis();
  //MotorTwo->setSpeed(250); 
  //MotorFour->setSpeed(250);
  /*if(team ==1){
    MotorTwo->run(FORWARD); 
    MotorFour->run(FORWARD); 
  }else{
    MotorTwo->run(BACKWARD); 
    MotorFour->run(BACKWARD);
  }*/
}
void msgInstru(String msg){
  if(state == WAIT and msg.indexOf("start") != -1){
    startRUNNING();
  }else if(state == RUN and msg.indexOf("start") != -1){
    state = END;
  }else if(state == WAIT and msg.indexOf("comm") != -1){
    printBLE("command ON");
    state = COMMAND;
    command = NO;
  }else if( msg.indexOf("debug") != -1){
    if(debug_state == DEBUG){
      debug_state = NODEBUG;
      printBLE("debug OFF");
    }else{
      debug_state = DEBUG;
      printBLE("debug ON");
    }
  }else if( msg.indexOf("state") != -1){
    if(state == WAIT){
      printBLE("state WAIT");
    }else if(state == RUN){
      printBLE("state RUN");
    }if(state == END){
      printBLE("state END");
    }if(state == COMMAND){
      printBLE("state COMMAND");
    }
    printBLE("state : "+String(state));
  }else if( msg.indexOf("team") != -1){
    printBLE("team : "+(team == 1)?"YELLOW":"BLUE");
  }else if(state == COMMAND and msg.indexOf("up") != -1){
    timeStartCOMMAND = millis();
    command = UP;
  }else if(state == COMMAND and msg.indexOf("down") != -1){
    timeStartCOMMAND = millis();
    command = DOWN;
  }else if(state == COMMAND and msg.indexOf("rotL") != -1){
    timeStartCOMMAND = millis();
    command = ROTLEFT;
  }else if(state == COMMAND and msg.indexOf("rotR") != -1){
    timeStartCOMMAND = millis();
    command = ROTRIGHT;
  }else if(state == COMMAND and msg.indexOf("comm") != -1){
    printBLE("command OFF");
    state = WAIT;
    command = NO;
  }else{
    printBLE("no valid");
  }
}
void printBLE(String msg){
    Wire.beginTransmission(ESP_addrI2C);
    for(int i=0; i < msg.length(); i++){
      Wire.write(msg[i]);
    }
    Wire.endTransmission(); 
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
