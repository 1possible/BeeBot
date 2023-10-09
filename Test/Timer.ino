int timeTest = 0;
bool flag = true;
float value = 3035;                   //Preload timer value (3035 for 4 seconds)


void setup() {
  
  noInterrupts();                       // disable all interrupts

  TCCR1A = 0;

  TCCR1B = 0;

  TCNT1 = value;                        // preload timer

  TCCR1B |= (1 << CS10)|(1 << CS12);    // 1024 prescaler 

  TIMSK1 |= (1 << TOIE1);               // enable timer overflow interrupt ISR

  interrupts();                         // enable all interrupts

  // Setup Serial Monitor
  Serial.begin(9600);




}

ISR(TIMER1_OVF_vect)                    // interrupt service routine for overflow

{

  TCNT1 = value;                                // preload timer

  //flag = false;
  timeTest ++;

}

void loop() {

  while(flag){
    Serial.println(timeTest);

    delay(100);
  }

}