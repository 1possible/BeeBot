#include "Encoder.h"

void Encoder::attachInterrupt() {
    pinMode(pin, INPUT);
    ::attachInterrupt(digitalPinToInterrupt(pin), Encoder::interruptHandlerStatic, CHANGE);
}

void Encoder::interruptHandler() {
    if (digitalRead(pin) == HIGH)
        count++;
    else
        count--;
}

// Static member function to act as interrupt handler
void Encoder::interruptHandlerStatic() {
    // Retrieve the instance of Encoder using a static pointer
    Encoder* instance = Encoder::getInstance();
    if (instance != nullptr) {
        instance->interruptHandler();
    }
}

// Static pointer to store the instance of Encoder
Encoder* Encoder::instance = nullptr;

// Function to retrieve the instance of Encoder
Encoder* Encoder::getInstance() {
    return instance;
}

// Constructor to set the instance of Encoder
Encoder::Encoder(int pin) {
    this->pin = pin;
    count = 0;
    // Set the instance of Encoder to this instance
    instance = this;
}
long Encoder::getCount() {
    return count;
}
