//GreatRobot code
#include <Wire.h>
#include <Adafruit_MotorShield.h>
//motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *MotorTwo = AFMS.getMotor(2);
Adafruit_DCMotor *MotorFour = AFMS.getMotor(4);
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
enum {TEAMCHOOSE , WAIT, RUN, END} state; 
enum {NODEBUG, DEBUG} debug_state;
int timeMsgDEBUG = 1000;
unsigned long timeStartRUN;
unsigned long timeStartDEBUG=0;
unsigned long timeNow;

//lcd
int team;

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
  state = TEAMCHOOSE;
  debug_state = NODEBUG;
}

void loop() {
  bool en_BLE = digitalRead(EN_BLE_PIN);
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
    case TEAMCHOOSE:
    {
  	  String readString;
      String Q;
      delay(1);
      if(Serial.available() > 0){
        char c = Serial.read();
        readString += c;
      }
      Q = readString;
      //YELLOW TEAM
      if (Q == "1"){
        team = 1;
        state = WAIT;
        printBLE("YELLOW Team");
      }
      //BLUE TEAM
      else if (Q == "2"){
        team = 2;
        state = WAIT;
        printBLE("Blue Team");
      }
      break;
    }
    case RUN:
    {
      //printBLE("RUN cc");
      timeNow = millis()-timeStartRUN;
      //printBLE("RUN "+String(timeNow)+" ms\n");
      if(timeNow >= 3000){
        state = END;
      }
      break;
    }
    case END:
    {
      printBLE("stop motor");
      MotorTwo->run(RELEASE); 
      MotorFour->run(RELEASE);
      state = TEAMCHOOSE;
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
  MotorTwo->setSpeed(250); 
  MotorFour->setSpeed(250);
  if(team ==1){
    MotorTwo->run(FORWARD); 
    MotorFour->run(FORWARD); 
  }else{
    MotorTwo->run(BACKWARD); 
    MotorFour->run(BACKWARD);
  }
}
void msgInstru(String msg){
  if(state == WAIT and msg.indexOf("start") != -1){
    startRUNNING();
  }else if( msg.indexOf("debug") != -1){
    if(debug_state == DEBUG){
      debug_state = NODEBUG;
      printBLE("debug OFF");
    }else{
      debug_state = DEBUG;
      printBLE("debug ON");
    }
  }else if( msg.indexOf("state") != -1){
    printBLE("state : "+String(state));
  }else if( msg.indexOf("team") != -1){
    printBLE("team : "+String(state));
  }
}
void printBLE(String msg){
    Wire.beginTransmission(ESP_addrI2C);
    for(int i=0; i < msg.length(); i++){
      Wire.write(msg[i]);
    }
    Wire.endTransmission(); 
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
