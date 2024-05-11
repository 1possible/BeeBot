#include "Motor.h"
#include "Movement.h"
#include "SonarSensor.h"



// DEFINE CONSTANTS    
const uint8_t HIGH_SPEED = 194;       // motor speeds   // befor Left at 194 and right at 196


// bug actuellement : 1. lorsque l'on change de d'équipe alors que le pami n'est pas alimenter celui si en fonction de la position va se metre à changer d'équipe à chaque fois qu'on le rallume et étteint
//                    2. à environ 4 sec de course l'un des moteurs du robot tourne moins vite que l'autre pendant 0.25s 


int iTest = 0;

#define switchGroup 3

#define startPin 8
#define IRleft_PIN 4  
#define IRright_PIN 5

enum {WAIT, RUN, TURN, END} state; 

int detectionLeft = HIGH;    // no obstacle
int detectionRight = HIGH; 

int v = 0;

unsigned long timeLine =0;
unsigned long timeStartPlay;

bool groupIsBlue;

float distance_mm = 0.0;
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN = 6;    
// const unsigned long MEASURE_TIMEOUT = 25000UL;
// const float SOUND_SPEED = 340.0 / 1000;
int notTurn = 0;


// DEFINE INSTANCES FOR CLASSES
static Motor motor; // Static instance of Motor
SonarSensor sonarSensor(TRIGGER_PIN, ECHO_PIN);


void setup() {
  Serial.begin(9600);
  motor.setupMotors();
  Movement::stopMovement();
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IRleft_PIN, INPUT); 
  pinMode(IRright_PIN, INPUT); 
  pinMode(startPin, INPUT_PULLUP);
  pinMode(switchGroup, INPUT_PULLUP);

  state = WAIT;
}

void loop() {
  switch(state){
    case WAIT:
      if(digitalRead(startPin) == HIGH){
        // Configuration for team colors and initial turning
        // configureTeamAndInitialTurn();
        delay(91000);
        timeStartPlay = millis();
        state = TURN;
      }
      break;
    case RUN:
      if (!moveUntilTargetOrTimeout()) {
        state = END;
      }
      else if (notTurn <=1 && (timeNow() >= 3600)){
        state = TURN;
      }
      break;
    case TURN:
      configureTeamAndTurn();
      notTurn++;
      state = RUN;
      break;
    case END:
      Movement::stopMovement();
      break;
  }
}

void configureTeamAndTurn() {

  if (digitalRead(switchGroup) == HIGH){
    groupIsBlue = true;
  }
  else{
    groupIsBlue = false;
  }
  // Définir les vitesses initiales et effectuer un virage initial léger
   // Vitesse pour le virage initial
  if (groupIsBlue) {
    // Si l'équipe est bleue, tourner légèrement à gauche
    Serial.println("Blue  group");     
    Movement::turnLeft();
  } else {
    // Si l'équipe est jaune, tourner légèrement à droite
    Serial.println("Yellow group");
    Serial.println(groupIsBlue);
    Movement::turnRight();
  }
  delay(330);  // Adjust delay to control the sharpness of the initial turn
  
}
long timeNow(){
  unsigned long timeNow = millis() - timeStartPlay;
  return timeNow;
}

bool moveUntilTargetOrTimeout() {
  if (timeNow() <= 8500) {
    distance_mm = sonarSensor.measureDistance();
    if (distance_mm != 0 && distance_mm < 120.0) {
      Serial.print("Obstacle at distance: ");
      Serial.println(distance_mm);
      return false;
    }
    Movement::forward();
    return true;
  } else {
    Serial.println("End due to timeout");
    return false;
  }
}


