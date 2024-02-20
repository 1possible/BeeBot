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

//char buff[48];
//int tail_buff = 0;
//int head_buff = 0;
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
  //Serial.println("loop time\n");
  /*if(Serial.available()){
    //ESP_BT.write(Serial.read());
    String dataReceived = Serial.readString();
    
    ESP_BT.print("Received:" + dataReceived);
    
  }*/
  if (ESP_BT.available()) { // Check if we receive anything from Bluetooth
    //Serial.write(ESP_BT.read());
    String dataReceived = ESP_BT.readString(); // Read what we receive
    ESP_BT.println("message recu");
    /*for(int i=0; i < dataReceived.length()-2; i++){
      buff[head_buff] = dataReceived[i];
      increaseHead();
      ESP_BT.println(head_buff);
    }*/
    int str_len = dataReceived.length();
    int str_modulo = str_len%6;
    if(str_modulo != 0){
      for(int i = 0; i < 6-str_modulo;i++){
        dataReceived+=" ";
        str_len++;
      }
    }
    char char_array[str_len];
    dataReceived.toCharArray(char_array, str_len);
    Wire.slaveWrite((uint8_t *) char_array, str_len);
    dataToSend += str_len;
    digitalWrite(I2C_Trig_PIN,HIGH);
    //ESP_BT.print("headbuff : ");
    //ESP_BT.println(head_buff);
    //ESP_BT.print("tailbuff : ");
    //ESP_BT.println(tail_buff);
    
    /*Serial.println("Received:" + dataReceived); // Print what we receive
    String dataReceived = ESP_BT.readString(); // Read what we receive
    Serial.println("Received:" + dataReceived); // Print what we receive

    // Extracting the float values
    int firstCommaIndex = dataReceived.indexOf(',');
    int secondCommaIndex = dataReceived.indexOf(',', firstCommaIndex + 1);

    String firstValue = dataReceived.substring(0, firstCommaIndex);
    String secondValue = dataReceived.substring(firstCommaIndex + 1, secondCommaIndex);
    String thirdValue = dataReceived.substring(secondCommaIndex + 1);

    float value1 = firstValue.toFloat();
    float value2 = secondValue.toFloat();
    float value3 = thirdValue.toFloat();

    // Print the extracted values
    Serial.print("Value 1: ");
    Serial.println(value1);
    Serial.print("Value 2: ");
    Serial.println(value2);
    Serial.print("Value 3: ");
    Serial.println(value3);

    // Send a response
    ESP_BT.println(dataReceived);*/
    
  }
  delay(20);
}
/*void increaseTail(){
  tail_buff++;
  if(tail_buff >= sizeof(buff)){
    tail_buff = 0;
  }
}
void increaseHead(){
  head_buff++;
  if(head_buff >= sizeof(buff)){
    head_buff = 0;
  }
  if(head_buff == tail_buff){
    error = true;
    ESP_BT.println("error : overflow buffer");
  }
}*/
void requestEvent()
{
  //Wire.write("hello ");
  //ESP_BT.println("message demandé");
  /*for(int i=0; i < 6; i++){
    if(head_buff != tail_buff){
      //Wire.slaveWrite((uint8_t *)buff[tail_buff],1); // respond with message of 6 bytes // as expected by master
      Wire.print(buff[tail_buff]);
      ESP_BT.println(buff[tail_buff]);
      increaseTail();
      ESP_BT.println(tail_buff);
    }else{
      Wire.print(" ");
    }
  }
  if(head_buff == tail_buff){
    digitalWrite(I2C_Trig_PIN,LOW);
  }*/
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
