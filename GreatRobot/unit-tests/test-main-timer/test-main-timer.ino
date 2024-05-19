#include <AUnit.h>
#include "..\..\GreatRobot\GreatRobot-Main\Timer.cpp"



// Test de la méthode setTimer()
test(TimerSetTimer) {
  Timer timer;
  timer.setup();
  timer.setTimer(1000); 
  assertTrue(timer.getElapsedTime() == 0);
  assertFalse(timer.endOfTimer());
}

// Test de la méthode endOfTimer()
test(TimerEndOfTimer) {
  Timer timer;
  timer.setup();
  timer.setTimer(1000); 
  
  delay(500);
  assertFalse(timer.endOfTimer());

  delay(600);
  assertTrue(timer.endOfTimer());
}

// Test de la méthode update()
test(TimerUpdate) {
  Timer timer;
  timer.setup();
  timer.setTimer(1000); 

  delay(500);
  assertTrue(timer.getElapsedTime() >= 500 && timer.getElapsedTime() < 600);
}

// Test de la méthode activate() and disable()
test(TimerActivate) {
  Timer timer;
  timer.setup();
  timer.setTimer(500); 
  timer.disable();
  delay(600);
  assertFalse(timer.endOfTimer());

  timer.activate();
  delay(600);
  assertTrue(timer.endOfTimer());
}

void setup() {
  #if ! defined(EPOXY_DUINO)
    delay(1000); // wait for stability on some boards to prevent garbage Serial
  #endif
    SERIAL_PORT_MONITOR.begin(9600); // ESP8266 default of 74880 not supported on Linux
    while (!SERIAL_PORT_MONITOR); // for the Arduino Leonardo/Micro only
  #if defined(EPOXY_DUINO)
    SERIAL_PORT_MONITOR.setLineModeUnix();
  #endif
  aunit::TestRunner::run();
}

void loop() {
  aunit::TestRunner::run();
}