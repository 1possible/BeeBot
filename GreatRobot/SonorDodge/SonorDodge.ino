#include <Adafruit_MotorShield.h>
//motor
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *MotorOne = AFMS.getMotor(1);
Adafruit_DCMotor *MotorFour = AFMS.getMotor(4);

enum {RUN , DETECT, DODGERIGHT, DODGELEFT} state; 

float distance_mm1 = 0.0;
float distance_mm2 = 0.0;

const byte TRIGGER_PIN = 7; 
const byte ECHO_PIN1 = 6;
const byte ECHO_PIN2 = 5;  
  
const unsigned long MEASURE_TIMEOUT = 22000UL;
const float SOUND_SPEED = 340.0 / 1000;

int timeRight = 0;
int timeLeft = 0;
int timeDodgeRight = 0;
int timeDodgeLeft = 0;

void setup() {
  /* Initialise le port série */
  Serial.begin(9600);
  AFMS.begin();
  /* Initialise les broches */
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN1, INPUT);
  /* Initialise les broches */
  pinMode(ECHO_PIN2, INPUT);
  /* Initialise les broches */
  
  state = RUN;
}

int measureDistance (const byte x, const byte y)
{
  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(x, HIGH);
  delayMicroseconds(10);
  digitalWrite(x, LOW);
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  unsigned long measure = pulseIn(y, HIGH, MEASURE_TIMEOUT);
  /* 3. Calcul la distance à partir du temps mesuré */
  int test = measure / 2.0 * SOUND_SPEED; 
  return test;
}

void loop() {
  
  switch(state){
    case RUN:
    {
      MotorOne->setSpeed(2200); 
      MotorFour->setSpeed(2200);
      MotorOne->run(FORWARD); 
      MotorFour->run(FORWARD);
      state = DETECT;
      break;
    }
    case DETECT:
    {
      distance_mm1 = measureDistance(TRIGGER_PIN, ECHO_PIN1);
      distance_mm2 = measureDistance(TRIGGER_PIN, ECHO_PIN2);
      Serial.println("distance_mm1");
      Serial.println(distance_mm1);
      Serial.println("distance_mm2");
      Serial.println(distance_mm2);
      //détecte une obstacle avant gauche
      if (distance_mm1 != 0 && distance_mm1 < 100.0){
        MotorOne->setSpeed(0); 
        MotorFour->setSpeed(0);
        MotorOne->run(RELEASE); 
        MotorFour->run(RELEASE);
        timeRight = 200;
        timeDodgeRight = 3;
        Serial.println("tourne droite");
        state = DODGERIGHT;
        break;
      }
      //détecte un obstacle avant droite
      else if (distance_mm2 != 0 && distance_mm2 < 100.0){
        MotorOne->setSpeed(0); 
        MotorFour->setSpeed(0);
        MotorOne->run(RELEASE); 
        MotorFour->run(RELEASE);
        timeLeft = 200;
        timeDodgeLeft = 3;
        state = DODGELEFT;
        break;
      }

      //tour par la droite
      if (timeRight > 1) {
        Serial.println("timeRight");
        Serial.println(timeRight);
        timeRight--;
        break;
      }
      else if (timeRight == 1){
        timeRight--;
        if (timeDodgeRight > 1){
          timeDodgeRight--;
          timeRight = 200;
          state = DODGELEFT;
          break;
        }
        else if (timeDodgeRight == 1){
          timeDodgeRight--;
          timeRight = 200;
          state = DODGERIGHT;
          break;
        }
        break;
      }

      //tour par la gauche
      if (timeLeft > 1) {
        timeLeft--;
        break;
      }
      else if (timeLeft == 1){
        timeLeft--;
        state = DODGELEFT;
        if (timeDodgeLeft > 1){
          timeDodgeLeft--;
          timeLeft = 200;
          state = DODGERIGHT;
          break;
        }
        else if (timeDodgeLeft == 1){
          timeDodgeLeft--;
          timeLeft = 200;
          state = DODGELEFT;
          break;
        }
        break;
      }
      delay(1);
      break;
    }
    case DODGERIGHT:
    {
      //angle droit
      MotorOne->setSpeed(2200); 
      MotorFour->setSpeed(2200);
      MotorOne->run(BACKWARD); 
      MotorFour->run(FORWARD);
      delay(10000);
      MotorOne->setSpeed(2200); 
      MotorFour->setSpeed(2200);
      MotorOne->run(FORWARD); 
      MotorFour->run(FORWARD);
      // timeDodgeRight += 1;
      state = DETECT;
      break;
    }
    case DODGELEFT:
    {
      //angle droit
      MotorOne->setSpeed(2200); 
      MotorFour->setSpeed(2200);
      MotorOne->run(FORWARD); 
      MotorFour->run(BACKWARD);
      delay(10000);
      MotorOne->setSpeed(2200); 
      MotorFour->setSpeed(2200);
      MotorOne->run(FORWARD); 
      MotorFour->run(FORWARD);
      
      // timeDodgeLeft += 1;
      state = DETECT;
      break;
    }
  }
}
