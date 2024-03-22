#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *MotorTwo = AFMS.getMotor(1);
Adafruit_DCMotor *MotorFour = AFMS.getMotor(4);

volatile long encoderCountMotorTwo = 0;
volatile long encoderCountMotorFour = 0;

// Paramètres de l'encodeur et de la roue
const float stepsPerRevolution = 360; // Nombre de pas par tour de l'encodeur
const float wheelDiameterCm = 10.0; // Diamètre de la roue en cm
const float wheelPerimeterCm = 3.14159 * wheelDiameterCm; // Périmètre de la roue

// Variables pour suivre la distance totale parcourue
float totalDistanceMotorTwo = 0.0;
float totalDistanceMotorFour = 0.0;

// Fonctions d'interruption pour les encodeurs
void encoderMotorTwoChange() {
  if (digitalRead(2) == HIGH)
    encoderCountMotorTwo++;
  else
    encoderCountMotorTwo--;
}

void encoderMotorFourChange() {
  if (digitalRead(3) == HIGH)
    encoderCountMotorFour++;
  else
    encoderCountMotorFour--;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Motor test!");

  AFMS.begin();

  pinMode(2, INPUT);
  pinMode(3, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), encoderMotorTwoChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), encoderMotorFourChange, CHANGE);

  Serial.println("Setup done.");
}

void loop() {
  MotorTwo->setSpeed(250);
  MotorFour->setSpeed(250);

  encoderCountMotorTwo = 0; // Réinitialiser le compteur d'encodeur
  encoderCountMotorFour = 0;

  MotorTwo->run(FORWARD);
  MotorFour->run(FORWARD);
  delay(1000); // Laisser les moteurs tourner pendant 1 seconde

  // Calculer la distance parcourue pendant le cycle actuel
  float distanceMotorTwo = (encoderCountMotorTwo / stepsPerRevolution) * wheelPerimeterCm;
  float distanceMotorFour = (encoderCountMotorFour / stepsPerRevolution) * wheelPerimeterCm;
  
  // Mettre à jour la distance totale parcourue
  totalDistanceMotorTwo += distanceMotorTwo;
  totalDistanceMotorFour += distanceMotorFour;

  // Afficher les distances
  Serial.print("Distance cycle Motor Two: ");
  Serial.print(distanceMotorTwo);
  Serial.println(" cm");
  Serial.print("Distance cycle Motor Four: ");
  Serial.print(distanceMotorFour);
  Serial.println(" cm");

  Serial.print("Total Distance Motor Two: ");
  Serial.print(totalDistanceMotorTwo);
  Serial.println(" cm");
  Serial.print("Total Distance Motor Four: ");
  Serial.print(totalDistanceMotorFour);
  Serial.println(" cm");

  MotorTwo->run(RELEASE);
  MotorFour->run(RELEASE);
  delay(1000); // Pause avant le prochain mouvement
}
