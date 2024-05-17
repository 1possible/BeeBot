# Communication I2C entre un Arduino et un ESP32

Ce projet démontre comment établir une communication I2C entre un Arduino et un ESP32. L'Arduino agira en tant que maître et l'ESP32 en tant qu'esclave. L'Arduino scanne d'abord les périphériques I2C, envoie un message à l'ESP32, et ensuite l'ESP32 répondra chaque seconde après avoir reçu le message initial.

## Matériel nécessaire

- 1x Arduino
- 1x ESP32
- Câbles de connexion

## Connexions matérielles

Les connexions I2C doivent être effectuées comme suit :

1. **GND de l'Arduino** connecté au **GND de l'ESP32**
2. **SDA de l'Arduino (A4)** connecté au **SDA de l'ESP32 (GPIO21)**
3. **SCL de l'Arduino (A5)** connecté au **SCL de l'ESP32 (GPIO22)**

![Schéma de connexion (src:https://docs.arduino.cc/learn/communication/wire/ )](image/image.png)
* dans le schema ci-dessous il est facile de remplacer l'un des deux arduinos par un esp32 (ou n'importe quelle autre board utilisant I2C, à condition de bien verifier dans la datsheet quelles pins sont SDA et SCL)*
## Configuration logicielle

### Code pour l'Arduino (Maître)

Ce code initialise l'I2C, scanne les périphériques I2C pour trouver l'ESP32, et envoie un message une fois qu'il est trouvé. Il affiche également tout message reçu de l'ESP32 sur le moniteur série.

```cpp
#include <Wire.h>

#define ESP32_ADDRESS 0x04
#define MASTER_ADDRESS 0x08
void receiveEvent(int howMany);

void setup() {
  Wire.begin(MASTER_ADDRESS);
  Wire.onReceive(receiveEvent); // Attach interrupt function
  Serial.begin(9600);
  
  // Scanning for I2C devices
  Serial.println("Scanning for I2C devices...");
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      
      if (address == ESP32_ADDRESS) {
        Serial.println("ESP32 found!");
        // Sending message to ESP32
        Wire.beginTransmission(ESP32_ADDRESS);
        Wire.write("Hello ESP32");
        Wire.endTransmission();
        Serial.println("Message sent to ESP32");
      }
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  Serial.println("Scanning done.");
}

void loop() {
  // Do nothing in the loop
}

void receiveEvent(int howMany) {
  String message = "";
  while (Wire.available()) {
    char c = Wire.read();
    message += c;
  }
  Serial.print("Message received: ");
  Serial.println(message);
}

```
dans le code ci-dessus les fonctions suivantes servent à :
#define ESP32_ADDRESS 0x04: Définition de l'adresse I2C de l'ESP32 (slave), important pour savoir à quelle adresse on doit envoyer des données
#define MASTER_ADDRESS 0x08; Définition de l'adresse I2C de l'arduino (Master), important pour que le slave puis s'adresser au Master. 
Wire.begin() :Initialisation de la communication I2C en tant que maître
Wire.onReceive(receiveEvent): Attache la fonction d'interruption pour réception de données I2C
Wire.available(): Vérifie si des données sont disponibles pour être lues
Wire.write(): Écrit les données dans le tampon de transmission I2C. Ces données seront envoyées au périphérique lors de l'appel à Wire.endTransmission().

### Code pour l'ESP32 (slave)

```cpp
#include <Wire.h>

#define SLAVE_ADDRESS 0x04
#define MASTER_ADDRESS 0x08 

bool messageReceived = false;

void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS); 
  Wire.onReceive(receiveEvent); 
}

void loop() {
  if (messageReceived) {
    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime >= 1000) {
      lastSendTime = millis();
      Wire.beginTransmission(MASTER_ADDRESS); 
      Wire.write("Response from ESP32");
      Wire.endTransmission();
      Serial.println("Response from ESP32");
    }
  }
  delay(100); 
}

void receiveEvent(int howMany) {
  while (Wire.available()) { 
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
  messageReceived = true;
}
```
Il est important de redéfinir les mêmes adresses que dans le code pour l'arduino pour que :
1. L'ESP32 soit à la bonne adresse
2. L'ESP32 sache à quelle adresse envoyer ses réponses