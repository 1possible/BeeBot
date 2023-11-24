#include <TimerOne.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *LeftMotor = AFMS.getMotor(2);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(3);

// Define variables
volatile bool timerExpired = false;
int count = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set up Timer1 to trigger the timerInterrupt function every 1000 milliseconds (1 second)
  Timer1.initialize(1000 * 1000); // 1000 microseconds = 1 millisecond
  Timer1.attachInterrupt(timerInterrupt);

  // Enable Timer1
  Timer1.start();

  // Your setup code can go here
}

void loop() {
  // Your main code goes here
  
  if (timerExpired) {
    Serial.println("10 seconds have passed!");
    LeftMotor->setSpeed(0);
    RightMotor->setSpeed(0);
    LeftMotor->run(RELEASE);
    RightMotor->run(RELEASE);

    
    // Perform any actions you want when the timer expires

    // Optional: You can reset the timer or take other actions here
    
    // Stop the loop
    while (true) {
      // Do nothing or perform any other necessary actions
    }
  }
  else{
    LeftMotor->run(FORWARD);
    RightMotor->run(FORWARD);
    LeftMotor->setSpeed(120);
    RightMotor->setSpeed(120);

    
  }
}

void timerInterrupt() {
  // This function is called by Timer1 interrupt every second
  

  count++;

  // Check if 10 seconds have passed
  if (count >= 400) {
    timerExpired = true;

    // Stop Timer1
    Timer1.stop();
  }
}