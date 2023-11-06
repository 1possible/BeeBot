#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);

int DIGITAL_OUT_L=2;
int DIGITAL_OUT_R=3;
int stepsL = 0;
int stepsR = 0;
int var = 0;
int Time = 0;
uint8_t speedR = 150;
uint8_t speedL = 150;

float TimValue = 960;
float distance_mm = 0.0;

const byte TRIGGER_PIN = 2; 
const byte ECHO_PIN = 3;    
const unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;

void checkStepStateL(){
  if (digitalRead(DIGITAL_OUT_L) == HIGH){
    stepsL +=1;
  }
}
void checkStepStateR(){
  if (digitalRead(DIGITAL_OUT_R) == HIGH){
    stepsR +=1;
  }
}

void setup() {
  // Set DIGITAL_OUT as an INPUT
  pinMode(DIGITAL_OUT_L, INPUT);
  pinMode(DIGITAL_OUT_R, INPUT);
  
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = TimValue;                     // preload timer
  TCCR1B |= (1 << CS10)|(1 << CS12);    // 1024 prescaler 
  TIMSK1 |= (1 << TOIE1);      
  interrupts();

  Serial.begin(9600);

  // attach Interrupt to Interrupt Service Routine
  attachInterrupt(digitalPinToInterrupt(DIGITAL_OUT_L), checkStepStateL, RISING);
  attachInterrupt(digitalPinToInterrupt(DIGITAL_OUT_R), checkStepStateR, RISING);
  
  if (!AFMS.begin()) {         
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  
  LeftMotor->setSpeed(150);
  LeftMotor->run(FORWARD);
  LeftMotor->run(RELEASE);
  RightMotor->setSpeed(150);
  RightMotor->run(FORWARD);
  RightMotor->run(RELEASE);

  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
}

ISR(TIMER1_OVF_vect)                    // interrupt service routine for overflow
{
  TCNT1 = value;                                // preload timer
  timeTest ++;

}

void loop() {

  measureDistance();
   switch(var){
    case 10.0 ... 19.9:
      LeftMotor->setSpeed(75);
      RightMotor->setSpeed(75);
      break;
    case 0.0 ... 9.9::
      LeftMotor->run(RELEASE);
      RightMotor->run(RELEASE);
      break;
    case default :
      LeftMotor->setSpeed(speedL);
      RightMotor->setSpeed(speedR);
      LeftMotor->run(FORWARD);
      RightMotor->run(FORWARD);
      break;
   }

}

void measureDistance 
{
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  unsigned long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
  distance_mm = measure / 2.0 * SOUND_SPEED;
}
