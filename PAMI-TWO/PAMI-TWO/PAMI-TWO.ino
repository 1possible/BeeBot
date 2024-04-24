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

enum {WAIT, RUN, TURN, END} state; 

int detectionLeft = HIGH;    // no obstacle
int detectionRight = HIGH; 

int vL = 0;
int VL = 0;
int vR = 0;
int VR = 0;
int v = 0;

unsigned long timeLine =0;
unsigned long timeStartPlay;

bool blueGroup;

float distance_mm = 0.0;
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN = 6;    
const unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;
int notTurn = 0;

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
      MoveStop();
      break;
  }
}

void configureTeamAndTurn() {

  if (digitalRead(switchGroup) == HIGH){
    blueGroup = true;
  }
  else{
    blueGroup = false;
  }
  // Définir les vitesses initiales et effectuer un virage initial léger
   // Vitesse pour le virage initial
  if (blueGroup) {
    // Si l'équipe est bleue, tourner légèrement à gauche
    Serial.println("Blue  group");
    RightMotor->setSpeed(67+90*notTurn);
    LeftMotor->setSpeed(0);
  } else {
    // Si l'équipe est jaune, tourner légèrement à droite
    Serial.println("Yellow group");
    Serial.println(blueGroup);
    LeftMotor->setSpeed(65+85*notTurn);
    RightMotor->setSpeed(0);
  }
  LeftMotor->run(FORWARD);
  RightMotor->run(FORWARD);
  delay(330);  // Adjust delay to control the sharpness of the initial turn

  // Set speeds for straight movement
  // vL = vR = v = 100;
  // VL = VR = 180;

  // VL = 230;
  // VR = 250;



  
}
long timeNow(){
  unsigned long timeNow = millis() - timeStartPlay;
  return timeNow;
}

bool moveUntilTargetOrTimeout() {
  
  if (timeNow() <= 8500) {
    measureDistance();
    if (distance_mm != 0 && distance_mm < 120.0) {
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
  vL=194;
  vR=196;
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
