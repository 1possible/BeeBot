#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);


// bug actuellement : 1. lorsque l'on change de d'équipe alors que le pami n'est pas alimenter celui si en fonction de la position va se metre à changer d'équipe à chaque fois qu'on le rallume et étteint
//                    2. à environ 4 sec de course l'un des moteurs du robot tourne moins vite que l'autre pendant 0.25s 


int iTest = 0;

#define switchGroup 3

#define startPin 8
#define IRleft_PIN 4  
#define IRright_PIN 5

enum {WAIT, PLAY, END} state; 

int detectionLeft = HIGH;    // no obstacle
int detectionRight = HIGH; 

int vL = 0;
int VL = 0;
int vR = 0;
int VR = 0;
int v = 0;

unsigned long timeLine =0;
unsigned long timeStartPlay;
unsigned long timeNow;

bool yellowGroup;

float distance_mm = 0.0;
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN = 6;    
const unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;

void setup() {
  Serial.begin(9600);
  if (!AFMS.begin()) {         
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  
  LeftMotor->setSpeed(0);
  LeftMotor->run(RELEASE);
  RightMotor->setSpeed(0);
  RightMotor->run(RELEASE);

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
        configureTeamAndInitialTurn();
        state = PLAY;
      }
      break;
    case PLAY:
      if (!moveUntilTargetOrTimeout()) {
        state = END;
      }
      break;
    case END:
      MoveStop();
      break;
  }
}

void configureTeamAndInitialTurn() {

  if (digitalRead(switchGroup) == HIGH){
    yellowGroup = true;
  }
  else{
    yellowGroup = false;
  }
  // Définir les vitesses initiales et effectuer un virage initial léger
  int initialTurnSpeed = 72; // Vitesse pour le virage initial
  if (yellowGroup) {
    // Si l'équipe est jaune, tourner légèrement à gauche
    Serial.println("Yellow group");
    RightMotor->setSpeed(initialTurnSpeed);
    LeftMotor->setSpeed(0);
  } else {
    // Si l'équipe est bleue, tourner légèrement à droite
    Serial.println("Blue group");
    LeftMotor->setSpeed(initialTurnSpeed);
    RightMotor->setSpeed(0);
  }
  LeftMotor->run(FORWARD);
  RightMotor->run(FORWARD);
  delay(500);  // Adjust delay to control the sharpness of the initial turn

  // Set speeds for straight movement
  // vL = vR = v = 100;
  // VL = VR = 180;

  // VL = 230;
  // VR = 250;

  vL=130;
  vR=149;

  timeStartPlay = millis();
}

bool moveUntilTargetOrTimeout() {
  unsigned long timeNow = millis() - timeStartPlay;
  if (timeNow <= 9500) {
    measureDistance();
    if (distance_mm != 0 && distance_mm < 100.0) {
      Serial.print("Obstacle at distance: ");
      Serial.println(distance_mm);
      return false;
    }
    MoveForward();
    return true;
  } else {
    Serial.println("End due to timeout");
    return false;
  }
}

void MoveStop() {
  LeftMotor->run(RELEASE);
  RightMotor->run(RELEASE);
}

void MoveForward() {
  LeftMotor->setSpeed(vL);
  RightMotor->setSpeed(vR);
  LeftMotor->run(FORWARD);
  RightMotor->run(FORWARD);
}

int measureDistance() {
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  unsigned long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
  distance_mm = measure / 2.0 * SOUND_SPEED;
  return distance_mm;
}
