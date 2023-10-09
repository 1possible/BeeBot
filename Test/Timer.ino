int timeTest = 0;
bool flag = true;                  //Preload timer value (31250 for 1/2 seconds)


void setup() {
  
  noInterrupts();                 // On désactive les interruptions, pour commencer

  // On règle les bits WGM10, WGM11, WGM12, WGM13 pour fonctionner en mode "CTC" (c'est à dire, en mode "comparaison timer <-> valeur de référence")
  bitClear(TCCR1A, WGM10);        // On met le bit WGM10 à 0 (contenu dans le registre TCCR1A)
  bitClear(TCCR1A, WGM11);        // On met le bit WGM11 à 0 (contenu dans le registre TCCR1A)
  bitSet(TCCR1B, WGM12);          // On met le bit WGM12 à 1 (contenu dans le registre TCCR1B)
  bitClear(TCCR1B, WGM13);        // On met le bit WGM13 à 0 (contenu dans le registre TCCR1B)

  // Ensuite, on règle les bits CS10, CS11, et CS12 pour que le prédiviseur du timer1 fasse une division par 256
  bitClear(TCCR1B, CS10);         // On met le bit CS10 à 0 (contenu dans le registre TCCR1B)
  bitClear(TCCR1B, CS11);         // On met le bit CS11 à 0 (contenu dans le registre TCCR1B)
  bitSet(TCCR1B, CS12);           // On met le bit CS12 à 1 (contenu dans le registre TCCR1B)

  // Puis on active l'interruption du timer1, qui test en permanence s'il y a égalité entre la valeur courant du timer, et la valeur
  // stockée dans un registre de comparaison. En pratique, pour ce faire, on va mettre à 1 le bit OCIE1A dans le registre TIMSK1,
  // afin qu'une interruption soit générée, à chaque fois que la valeur du timer1 sera égale à la valeur qu'on aura renseigné dans le registre OCR1A
  bitSet(TIMSK1, OCIE1A);         // On met le bit OCIE1A à 1 (contenu dans le registre TIMSK1)

        /* Pour info, on aura pu simplifier l'écriture des lignes ci-dessus, en entrant directement la valeur de tous les bits à la fois, 
         * dans leurs registres correspondants. Pour cela, il aurait fallut écrire les lignes suivantes (au lieu de bitClear/bitSet) : 
         * 
         *      TCCR1A = 0b00000000;      // pour WGM11=0 et WGM10=0
         *      TCCR1B = 0b00001100;      // pour WGM12=1 et WGM13=0, puis CS12=1/CS11=0/CS10=0 pour prédiviseur réglé sur division par 256
         *      TIMSK1 = 0b00000010;      // pour OCIE1A=1, afin d'activer l'interruption par comparaison "A" (test d'égalité entre timer et valeur de registre OCIE1A)
         */


  // Enfin, on met le compteur à zéro, on entre la valeur déclenchant l'interruption (nos "31250" coups d'horloge), et on réactive les interruptions
  TCNT1 = 0;            // Mise du timer1 à zéro
  OCR1A = 31250;        // Valeur correspondant à 500ms (car 31250 fois 16 µS donne bien 500ms ; pour rappel, ces 16 µS proviennent du calcul 1/(16MHz/256),
                        // avec les 16 MHz correspondant à la fréquence du quartz de l'ATmega328P, et le 256 au réglage du prédiviseur du timer1 fait précédemment)
  interrupts();         // Et, on ré-active les interruptions                     // enable all interrupts

  // Setup Serial Monitor
  Serial.begin(9600);




}

ISR(TIMER1_COMPA_vect)                    // interrupt service routine for overflow

{
  //flag = false;
  timeTest ++;

}

void loop() {

  while(flag){
    Serial.println(timeTest);

    delay(10);
  }

}
