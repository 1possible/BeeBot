//GreatRobot code
int team;
int state = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  // pinMode(12, INPUT);
  // pinMode(11, INPUT);
  Serial.begin(9600);
}

void loop() {  
  switch(state){
    //Team choise
    case 0:
    {
      String readString;
      String Q;
      delay(1);
      if(Serial.available() > 0){
        char c = Serial.read();
        readString += c;
      }
      Q = readString;
      //RED TEAM
      if (Q == "1"){
        team = 1;
        digitalWrite(13, HIGH);
        state = 1;
        break;
      }
      //BLUE TEAM
      if (Q == "2"){
        team = 2;
        digitalWrite(13, LOW);
        state = 2;
        break;
      }
      break;
    }
    case 1:
    {
      //code for RED team
      break;
    }
    case 2:
    {
      //code for BLUE team
      break;
    }
  }
}
