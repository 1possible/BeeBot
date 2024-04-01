#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
  private:
    int pin;
    volatile long count;
    static Encoder* instance; // Static member to store the instance

  public:
    Encoder(int pin);
    void attachInterrupt();
    void interruptHandler();
    static void interruptHandlerStatic(); // Static member function for interrupt handler
    static Encoder* getInstance(); // Static member function to get the instance
    long getCount();
};

#endif
