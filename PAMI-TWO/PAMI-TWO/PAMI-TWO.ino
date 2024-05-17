#include "Motor.h"
#include "Movement.h"
#include "SonarSensor.h"
#define switchGroup 3
#define startPin 8


// Pins
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN = 6;   


// DEFINE CONSTANTS    
const uint8_t HIGH_SPEED = 194;       // motor speed   // befor Left at 194 and right at 196
unsigned long timeLine =0;
unsigned long timeStartPlay;
bool groupIsBlue;
int notTurn = 0;
enum {WAIT, RUN, TURN, END} state; 


// DEFINE INSTANCES FOR CLASSES
static Motor motor;
SonarSensor sonarSensor(TRIGGER_PIN, ECHO_PIN);


void setup() {
  Serial.begin(9600);
  motor.setupMotors();
  Movement::stopMovement();
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(switchGroup, INPUT_PULLUP);
  state = WAIT;
}

void loop() {
  switch(state){
    case WAIT:
      if(digitalRead(startPin) == HIGH){
        // Configuration for team colors 
        if (digitalRead(switchGroup) == HIGH){
          groupIsBlue = true;
          Serial.println("Blue group"); 
          }
        else{
          groupIsBlue = false;
          Serial.println("Yellow group");
          }
        delay(91000);
        timeStartPlay = millis();
        state = TURN;
      }
      break;
    case RUN:
      unsigned long timeNow = millis() - timeStartPlay;
      if (timeNow <= 8500) {
        if (sonarSensor.obstacleInTheWay()){
          state = END;
        }
        else {
          Movement::forward();
          if (notTurn <=1 && (timeNow >= 3600)){
            state = TURN;
          }
        }
      }
      else {
        state = END;
      }
      break;

    case TURN:
      // If the team is blue, turn sligthly to the left
      if (groupIsBlue) {
        Movement::turnLeft();
      } 
      // If the team is Yellow, turn sligthly to the right
      else {
        Movement::turnRight();
      }
      delay(330);  // Adjust delay to control the sharpness of the initial turn
      notTurn++;
      state = RUN;
      break;

    case END:
      Movement::stopMovement();
      break;
  }
}

// bug actuellement : 1. lorsque l'on change de d'équipe alors que le pami n'est pas alimenter celui si en fonction de la position va se metre à changer d'équipe à chaque fois qu'on le rallume et étteint
//                    2. à environ 4 sec de course l'un des moteurs du robot tourne moins vite que l'autre pendant 0.25s 
