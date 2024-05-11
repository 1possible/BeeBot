#include "SonarSensor.h"

SonarSensor::SonarSensor(byte triggerPin, byte echoPin)
    : _triggerPin(triggerPin), _echoPin(echoPin) {}


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

bool SonarSensor::obstacleInTheWay() {
    distance_mm = measureDistance();
    if (distance_mm != 0 && distance_mm < 100.0) {
      Serial.print("Obstacle at distance: ");
      Serial.println(distance_mm);
      return true;
    }
    else {
      return false;
    }
}