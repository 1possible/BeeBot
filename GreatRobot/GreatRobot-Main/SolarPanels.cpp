#include "SolarPanels.h"

Motor SolarPanels::motor; 
SolarPanels::SolarPanels() : sonarSensor(TRIGGER_PIN, ECHO_PIN_SolarPan) {}

void SolarPanels::setupSolarPanels() {
    sonarSensor.setup();
}

// Define an enumeration for different states
enum SolarPanelState {
    STATE_FORWARD,
    STATE_DETECT_SOLAR_PANEL,
    STATE_TURN_SOLAR_PANEL
};

SolarPanelState currentState = STATE_FORWARD;
int iterationCount = 0;
const int MAX_ITERATIONS = 3;
float distanceFromSolarPanel;

void SolarPanels::play() {
    while (iterationCount < MAX_ITERATIONS) {
        switch (currentState) {
            case STATE_FORWARD:
                Serial.println("State: FORWARD");
                Movement::backward();
                
                // Check if solar panel is detected
                distanceFromSolarPanel = sonarSensor.measureDistance();
                Serial.println("Distance from solar panel: " + String(distanceFromSolarPanel));
                
                // If solar panel is detected, transition to STATE_TURN_SOLAR_PANEL
                if (distanceFromSolarPanel >= 100 && distanceFromSolarPanel <= 200) {
                    Serial.println("Solar panel detected");
                    currentState = STATE_TURN_SOLAR_PANEL;
                    iterationCount++;
                }
                break;

            case STATE_TURN_SOLAR_PANEL:
                Serial.println("State: TURN_SOLAR_PANEL");
                motor.turnSolarPanel();
                currentState = STATE_FORWARD;
                break;
        }
        
    

        delay(100);     // Add a delay to avoid continuous checking
    }
}
