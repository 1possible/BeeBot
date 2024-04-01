#ifndef ENCODERLOGIC_H
#define ENCODERLOGIC_H

#include "Encoder.h"

class EncoderLogic {
  private:
    Encoder motorTwo;
    Encoder motorFour;
  
  public:
    EncoderLogic(int pinMotorTwo, int pinMotorFour);
    void update();
    float getDistanceMotorTwo();
    float getDistanceMotorFour();
    float getTotalDistanceMotorTwo();
    float getTotalDistanceMotorFour();
};

#endif
