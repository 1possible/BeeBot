#include "EncoderLogic.h"

const float stepsPerRevolution = 360; // Number of steps per revolution
const float wheelDiameterCm = 4.5; // Diameter of the wheel in cm
const float wheelPerimeterCm = 3.14159 * wheelDiameterCm; // Wheel perimeter

float totalDistanceMotorTwo = 0.0;
float totalDistanceMotorFour = 0.0;

EncoderLogic::EncoderLogic(int pinMotorTwo, int pinMotorFour) : motorTwo(pinMotorTwo), motorFour(pinMotorFour) {
  motorTwo.attachInterrupt();
  motorFour.attachInterrupt();
}

void EncoderLogic::update() {
  // Update total distances
  totalDistanceMotorTwo += (motorTwo.getCount() / stepsPerRevolution) * wheelPerimeterCm;
  totalDistanceMotorFour += (motorFour.getCount() / stepsPerRevolution) * wheelPerimeterCm;
}

float EncoderLogic::getDistanceMotorTwo() {
  return (motorTwo.getCount() / stepsPerRevolution) * wheelPerimeterCm;
}

float EncoderLogic::getDistanceMotorFour() {
  return (motorFour.getCount() / stepsPerRevolution) * wheelPerimeterCm;
}

float EncoderLogic::getTotalDistanceMotorTwo() {
  return totalDistanceMotorTwo;
}

float EncoderLogic::getTotalDistanceMotorFour() {
  return totalDistanceMotorFour;
}
