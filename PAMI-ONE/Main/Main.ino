#include "Motor.h"
#include "Movement.h"
#include "SonarSensor.h"
#define switchGroup 3
#define startPin 8


// DEFINE PINS
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN = 6;  


// DEFINE INSTANCES FOR CLASSES
static Motor motor;
SonarSensor sonarSensor(TRIGGER_PIN, ECHO_PIN);


// DEFINE CONSTANTS    
const uint8_t HIGH_SPEED = 130;       // motor speed
unsigned long timeLine =0;
unsigned long timeStartPlay;
unsigned long timeNow;
enum {WAIT, PLAY, END} state; 


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
        // Configuration for team colors and initial turning
        // If the team is Yellow, turn sligthly to the right
        if (digitalRead(switchGroup) == HIGH){
          Serial.println("Yellow group");
          Movement::turnLeft();
        }
        // If the team is blue, turn sligthly to the left
        else{
          Serial.println("Blue group");
          Movement::turnRight();
        }
        delay(500);  // Adjust delay to control the sharpness of the initial turn
        timeStartPlay = millis();
        state = PLAY;
      }
      break;

    case PLAY:
      unsigned long timeNow = millis() - timeStartPlay;
      if (timeNow <= 9500) {
        if (sonarSensor.obstacleInTheWay()) {
          state = END;
        }
        else{
          Movement::forward();
        }
      } 
      else {
        Serial.println("End due to timeout");
        state = END;
      }
      break;

    case END:
      Movement::stopMovement();
      break;
  }
}


// bug actuellement : 1. lorsque l'on change de d'équipe alors que le pami n'est pas alimenter celui si en fonction de la position va se metre à changer d'équipe à chaque fois qu'on le rallume et étteint
//                    2. à environ 4 sec de course l'un des moteurs du robot tourne moins vite que l'autre pendant 0.25s 



