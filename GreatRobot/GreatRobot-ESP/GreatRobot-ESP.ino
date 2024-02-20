#include "BluetoothSerial.h"
//code exemple balise : https://github.com/TardyNoe/Balise/blob/main/Esp/BLEExample
// https://www.aranacorp.com/fr/communication-bluetooth-avec-esp32/
BluetoothSerial ESP_BT; // Object for Bluetooth
//https://assiss.github.io/arduino-zhcn/cn/Tutorial/MasterReader.html
//https://docs.espressif.com/projects/arduino-esp32/en/latest/api/i2c.html

//esp slave
//https://forum.arduino.cc/t/understanding-esp32-wireslave-example/1000680/7
//https://docs.espressif.com/projects/arduino-esp32/en/latest/api/i2c.html
#include <Wire.h>

const int I2C_SDA_PIN = 14;
const int I2C_SCL_PIN = 27;
const int I2C_Trig_PIN = 12;

int dataToSend = 0;
int lengthPayload = 6;

bool error= false;

void setup() {
  Serial.begin(115200);
  ESP_BT.begin("BEEBOT-ESP32_BT"); // Name of your Bluetooth signal
  Serial.print("Bluetooth device is ready to pair \n");
  pinMode(I2C_Trig_PIN,OUTPUT);
  Wire.setPins(I2C_SDA_PIN,I2C_SCL_PIN);
  Wire.begin(3);
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);
}

void loop() {
  if (ESP_BT.available()) { // Check if we receive anything from Bluetooth
    String dataReceived = ESP_BT.readString(); // Read what we receive
    //ESP_BT.println("message recu");
    // change the length of the data to be in the payload length
    int str_len = dataReceived.length();
    int str_modulo = str_len%6;
    if(str_modulo != 0){
      for(int i = 0; i < 6-str_modulo;i++){
        dataReceived+=" ";
        str_len++;
      }
    }
    //change format of data
    char char_array[str_len];
    dataReceived.toCharArray(char_array, str_len);
    
    Wire.slaveWrite((uint8_t *) char_array, str_len);//write the data in the I2C buffer
    dataToSend += str_len;
    digitalWrite(I2C_Trig_PIN,HIGH);
  }
  //delay(20);
}
void requestEvent()
{
  //Wire.write("hello ");
  //ESP_BT.println("message demandé");
  dataToSend = dataToSend - lengthPayload;
  if(dataToSend <= 0){
    digitalWrite(I2C_Trig_PIN,LOW);
    dataToSend = 0;
  }
}
void receiveEvent(int howMany)
{
  while(Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    ESP_BT.print(c);         // print the character
  }
}
