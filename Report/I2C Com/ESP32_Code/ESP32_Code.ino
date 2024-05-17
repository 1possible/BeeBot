#include <Wire.h>

#define SLAVE_ADDRESS 0x04
#define MASTER_ADDRESS 0x08 // Ajoutez l'adresse du maître ici

bool messageReceived = false;

void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS); // Initialisation de l'ESP32 en tant qu'esclave
  Wire.onReceive(receiveEvent); // Attache la fonction d'interruption pour réception de données I2C
}

void loop() {
  if (messageReceived) {
    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime >= 1000) {
      lastSendTime = millis();
      Wire.beginTransmission(MASTER_ADDRESS); // Envoie le message au maître (Arduino)
      Wire.write("Response from ESP32");
      Wire.endTransmission();
      Serial.println("Response from ESP32");
    }
  }
  delay(100); // Juste pour laisser le temps à l'événement de réception de se traiter
}

void receiveEvent(int howMany) {
  while (Wire.available()) { // Vérifie si des données sont disponibles pour être lues
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
  messageReceived = true; // Met à jour le drapeau une fois qu'un message est reçu
}
