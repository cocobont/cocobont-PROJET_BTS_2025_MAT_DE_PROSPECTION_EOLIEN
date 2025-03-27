
// WindStation.ino
#include "WindSensor.h"

WindSensor windSensor(13, 12);
unsigned long lastPrintTime = 0;
void setup() {
    windSensor.begin();
}

void loop() {
    windSensor.update();
    unsigned long currentTime = millis();
    if (currentTime - lastPrintTime >= 30000) { //120000 pour 2 min
    Serial.print("Vitesse du vent : ");
    Serial.print(windSensor.getWindSpeed());
    Serial.println(" km/h");
    Serial.println(" N = 1, N-E = 2, E = 3, S-E = 4, S = 5, S-O = 6, O = 7, N-O = 8 ");
    Serial.print("Direction du vent : ");
    Serial.println(windSensor.getWindDirection());

    Serial.println("-----------------------------------");
    lastPrintTime = currentTime;
    }

}
