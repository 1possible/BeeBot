#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(3);

int DIGITAL_OUT_L=2;
int DIGITAL_OUT_R=3;
int stepsL = 0;
int stepsR = 0;
// Constant for steps in disk
const float stepcount = 20.00;
// Constant for wheel diameter
const float wheeldiameter = 6.61;
// Integers for pulse counters
volatile int counter_A = 0;
volatile int counter_B = 0;

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

//transform cm into number of steps
int CMtoSteps(float cm) {

  int result;  // Final calculation result
  float circumference = (wheeldiameter * 3.14); // Calculate wheel circumference in cm
  float cm_step = circumference / stepcount;  // CM per Step
  
  float f_result = 2*(cm / cm_step);  // Calculate result as a float
  result = (int) f_result; // Convert to an integer (note this is NOT rounded)
  
  return result;  // End and return result

}

void MoveForward(int steps, int mspeed) 
{
   stepsL = 0;  //  reset counter A to zero
   stepsR = 0;  //  reset counter B to zero
   
   // Set Motors forward
   LeftMotor->run(BACKWARD);
   RightMotor->run(FORWARD);

   
   // Go forward until step value is reached
   while (steps > stepsL && steps > stepsR) {
   
    if (steps > stepsL) {
      LeftMotor->setSpeed(mspeed);  
    } else {
      LeftMotor->setSpeed(0);
    }
    if (steps > stepsR) {
      RightMotor->setSpeed(mspeed);
    } else {
      RightMotor->setSpeed(0);
    }
   }
    
  // Stop when done
  LeftMotor->run(RELEASE);
  RightMotor->run(RELEASE);
  stepsL = 0;  //  reset counter A to zero
  stepsR = 0;  //  reset counter B to zero 

}

void setup() {

  // Set DIGITAL_OUT as an INPUT
  pinMode(DIGITAL_OUT_L, INPUT);
  pinMode(DIGITAL_OUT_R, INPUT);

  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  attachInterrupt(digitalPinToInterrupt(DIGITAL_OUT_L), checkStepStateL, RISING);
  attachInterrupt(digitalPinToInterrupt(DIGITAL_OUT_R), checkStepStateR, RISING);

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  delay(2000); //delay 2s
  Serial.println(CMtoSteps(50));
  
  MoveForward(240,120); //moveforward 50cm at 30 speed CMtoSteps(50)
  Serial.println("Finsih mouving forward");

  // LeftMotor->setSpeed(150);
  // LeftMotor->run(FORWARD);
  // // turn on motor
  // LeftMotor->run(RELEASE);

  // RightMotor->setSpeed(150);
  // RightMotor->run(FORWARD);
  // // turn on motor
  // RightMotor->run(RELEASE);
}

void loop() {

  // put your main code here, to run repeatedly:
  //   uint8_t i;

  // Serial.println("tick");
  // Serial.println(stepsL);
  // Serial.println(stepsR);

  // LeftMotor->run(FORWARD);
  // RightMotor->run(BACKWARD);
  // for (i=0; i<255; i++) {
  //   LeftMotor->setSpeed(i);
  //   RightMotor->setSpeed(i);
  //   delay(10);
  // }
  // Serial.println("tOck");
  // Serial.println(stepsL);
  // Serial.println(stepsR);

  // for (i=255; i!=0; i--) {
  //   LeftMotor->setSpeed(i);
  //   RightMotor->setSpeed(i);
  //   delay(10);
  // }

  // Serial.println("tock");
  // Serial.println(stepsL);
  // Serial.println(stepsR);

  // LeftMotor->run(BACKWARD);
  // RightMotor->run(FORWARD);
  // for (i=0; i<255; i++) {
  //   LeftMotor->setSpeed(i);
  //   RightMotor->setSpeed(i);
  //   delay(10);
  // }
  // for (i=255; i!=0; i--) {
  //   LeftMotor->setSpeed(i);
  //   RightMotor->setSpeed(i);
  //   delay(10);
  // }

  // Serial.println("tech");
  // Serial.println(stepsL);
  // Serial.println(stepsR);

  // LeftMotor->run(RELEASE);
  // RightMotor->run(RELEASE);
  // delay(1000);

}
