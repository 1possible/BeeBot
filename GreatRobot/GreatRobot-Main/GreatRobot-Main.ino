 //GreatRobot code
#include <Wire.h>
#include <Adafruit_MotorShield.h>
//motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(3);
int vL = 75; //Low speed left //75
int VL = 250; //Hight speed left
int vR = 75; //Low speed right //50
int VR = 250; //Hight speed right
int v = 250; //speed forward 

//pin sonor sensor
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN1 = 6;

//IR sensor
#define IR_left_PIN 9
#define IR_right_PIN 10

enum { WAIT, RUN, DODGERIGHT, DODGELEFT, END} state; 

unsigned long timeNow;
unsigned long timeStartRUN;
unsigned long timeLine = 0;

//lcd
int team = 1; // 1 YELLOW Team 2 BLUE

//sonor sensor constant
float distance_mm1 = 0.0;
const unsigned long MEASURE_TIMEOUT = 22000UL;
const float SOUND_SPEED = 340.0 / 1000;
int timeRight = 0;
int timeDodgeRight = 0;

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
  //serial (comm ard-lcd)
  Serial.begin(9600);
  //Serial.print("setup...");

  //IR MOTOR
  pinMode(IR_left_PIN, INPUT);
  pinMode(IR_right_PIN, INPUT);

  //sonor
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN1, INPUT);
  
  //statemachine
  state = WAIT;
}

void loop() {
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
      distance_mm1 = measureDistance(TRIGGER_PIN, ECHO_PIN1);
      if(timeNow >= 120000){
        state = END;
      }
      else{
        if (distance_mm1 != 0 && distance_mm1 < 100.0){
          RightMotor->setSpeed(0); 
          LeftMotor->setSpeed(0);
          RightMotor->run(RELEASE); 
          LeftMotor->run(RELEASE);
          timeRight = 80;
          timeDodgeRight = 3;
          Serial.println("tourne droite");
          state = DODGERIGHT;
          break;
        }
        if (timeDodgeRight > 0){
          //tour par la droite
          if (timeRight > 1) {
            Serial.println("timeRight");
            Serial.println(timeRight);
            timeRight--;
            break;
          }
          else if (timeRight == 1){
            timeRight--;
            if (timeDodgeRight > 1){
              timeDodgeRight--;
              timeRight = 80;
              state = DODGELEFT;
              break;
            }
            else if (timeDodgeRight == 1){
              timeDodgeRight--;
              timeRight = 80;
              state = DODGERIGHT;
              break;
            }
            break;
          }
        }
        else{
          bool endline =followingLine();
          if(endline){
            state = END;
          }
        }
        
      }
      break;
    }
   case DODGERIGHT:
    {
      //angle droit
      RightMotor->setSpeed(225); 
      LeftMotor->setSpeed(225);
      RightMotor->run(BACKWARD); 
      LeftMotor->run(FORWARD);
      delay(3955);
      RightMotor->setSpeed(225); 
      LeftMotor->setSpeed(225);
      RightMotor->run(FORWARD); 
      LeftMotor->run(FORWARD);
      // timeDodgeRight += 1;
      state = RUN;
      break;
    }
    case DODGELEFT:
    {
      //angle droit
      RightMotor->setSpeed(225); 
      LeftMotor->setSpeed(225);
      RightMotor->run(FORWARD); 
      LeftMotor->run(BACKWARD);
      delay(3800);
      RightMotor->setSpeed(225); 
      LeftMotor->setSpeed(225);
      RightMotor->run(FORWARD); 
      LeftMotor->run(FORWARD);
      
      // timeDodgeLeft += 1;
      state = RUN;
      break;
    }
    case END:
    {
      RightMotor->run(RELEASE); 
      LeftMotor->run(RELEASE);
      state = WAIT;
      //Serial.println("END");
      break;
    }
  }
  delay(50);
}
void startRUN(){
  state = RUN;
  //Serial.println("RUN");
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
