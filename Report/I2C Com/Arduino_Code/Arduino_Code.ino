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
