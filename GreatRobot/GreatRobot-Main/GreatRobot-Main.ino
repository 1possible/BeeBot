//GreatRobot code
#include <Wire.h>

//sensor distance
/*float distance_mm = 0.0;
const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN = 6;   
const unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;
*/
enum {WAIT, RUN , END} state; 
unsigned long timeStartRUN;
unsigned long timeNow;

//esp BLE
const byte EN_BLE_PIN = 8; 
const int ESP_addrI2C = 3;
String messageBLE = "";

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);

  /*pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);*/

  pinMode(EN_BLE_PIN,INPUT);
  state = WAIT;
}

void loop() {
  bool en_BLE = digitalRead(EN_BLE_PIN);
  if(en_BLE == HIGH){
    Serial.println("comm esp");
    Wire.requestFrom(ESP_addrI2C,6);
    while(Wire.available()){
      char c = Wire.read();
      messageBLE+= c;
    }
    msgInstru(messageBLE);
  }
  /*if(Serial.available()){
    Serial.print("en_pin");
    Serial.println(en_BLE);
    String dataReceived = Serial.readString();
    Wire.beginTransmission(ESP_addrI2C);
    for(int i=0; i < dataReceived.length(); i++){
      Wire.write(dataReceived[i]);
    }
    Wire.endTransmission(); 
  }*/
    
  /*measureDistance();
  Serial.print(distance_mm);
  Serial.println(" mm");*/
  switch(state){
    case RUN:
      timeNow = millis()-timeStartRUN;
      if(timeNow >= 3000){
        state = END;
      }
      break;
    case END:
      printBLE("stop motor");
      state = WAIT;
      break;
      
  }
  delay(50);

}
void msgInstru(String msg){
  if(state == WAIT and msg.indexOf("start") != -1){
    state = RUN;
    printBLE("start running");
    timeStartRUN=millis();
  }
}
void printBLE(String msg){
    Wire.beginTransmission(ESP_addrI2C);
    for(int i=0; i < msg.length(); i++){
      Wire.write(msg[i]);
    }
    Wire.endTransmission(); 
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
