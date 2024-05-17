#include "SonarSensor.h"

SonarSensor::SonarSensor(byte triggerPin, byte echoPin)
    : _triggerPin(triggerPin), _echoPin(echoPin) {}

void SonarSensor::setup(int newValDistDetect, unsigned long newHoldTime) {
    pinMode(_triggerPin, OUTPUT);
    digitalWrite(_triggerPin, LOW); // Trigger pin should be LOW initially
    pinMode(_echoPin, INPUT);
    distDetect = newValDistDetect;
    distance = 0;
    timeLastDetection = 0;
    holdTime = newHoldTime;
}

void SonarSensor::setDistanceDetection(int newValDistDetect){
  distDetect = newValDistDetect;
}
void SonarSensor::setHoldTime(int newHoldTime){
  holdTime = newHoldTime;
}

// Send the distance in mm
float SonarSensor::measureDistance() {
    /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_triggerPin, LOW);
    /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
    unsigned long duration = pulseIn(_echoPin, HIGH, _MEASURE_TIMEOUT);
    /* 3. Calcul la distance à partir du temps mesuré */
    return duration / 2.0 * _SOUND_SPEED;
}

bool SonarSensor::detection(){
  distance = measureDistance();
  if(distance != 0 & distance < distDetect){
    val_detect = true;
    timeLastDetection = millis();
  }
  else if(val_detect & millis()-timeLastDetection >holdTime){
    val_detect = false;
  }
  return val_detect;
}

float SonarSensor::getDistance(){
  return distance;
}
