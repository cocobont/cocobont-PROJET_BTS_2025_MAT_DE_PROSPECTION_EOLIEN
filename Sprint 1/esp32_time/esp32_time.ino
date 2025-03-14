#include <ESP32Time.h>

ESP32Time rtc(0);  // Pas de décalage pour éviter les erreurs

void setup() {
  Serial.begin(115200);
  Serial.println("Entrez la date et l'heure sous le format JJ/MM/AAAA HH:MM:SS");

  while (!Serial.available()) {
    delay(100);
  }

  String input = Serial.readStringUntil('\n');
  input.trim();

  int day, month, year, hour, minute, second;
  if (sscanf(input.c_str(), "%d/%d/%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second) == 6) {
    rtc.setTime(second, minute, hour, day, month, year);
    Serial.println("Horloge réglée !");
  } else {
    Serial.println("Format invalide, redémarrez l'ESP32 et réessayez.");
  }
}

void loop() {
  // Simule la réception de données
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    receivedData.trim();

    if (receivedData.length() > 0) {
      Serial.print("Donnée reçue: ");
      Serial.print(receivedData);
      Serial.print(" - Horodatage: ");
      Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
    }
  }

  delay(100);
}
