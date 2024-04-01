#include "Encoder.h"

extern volatile long encoderCountMotorTwo;
extern volatile long encoderCountMotorFour;

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
