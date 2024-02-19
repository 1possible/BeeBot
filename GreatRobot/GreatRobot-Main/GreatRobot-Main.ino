//GreatRobot code
#include <Wire.h>

//sensor distance
/*float distance_mm = 0.0;
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN = 6;   
const unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;
*/
//esp BLE
const byte EN_BLE_PIN = 8; 
const int ESP_addrI2C = 3;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);

  /*pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);*/

  pinMode(EN_BLE_PIN,INPUT);
}

void loop() {
  bool en_BLE = digitalRead(EN_BLE_PIN);
  if(en_BLE == HIGH){
    Serial.println("comm esp");
    Wire.requestFrom(ESP_addrI2C,6);
    while(Wire.available()){
      char c = Wire.read();
      Serial.print(c);
    }
    Serial.println();
  }
  if(Serial.available()){
    Serial.print("en_pin");
    Serial.println(en_BLE);
    String dataReceived = Serial.readString();
    Wire.beginTransmission(ESP_addrI2C);
    for(int i=0; i < dataReceived.length(); i++){
      Wire.write(dataReceived[i]);
    }
    Wire.endTransmission(); 
  }
    
  /*measureDistance();
  Serial.print(distance_mm);
  Serial.println(" mm");*/
  delay(50);

}

/*int measureDistance ()
{
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  unsigned long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
  distance_mm = measure / 2.0 * SOUND_SPEED;
  return distance_mm ; 
}*/
