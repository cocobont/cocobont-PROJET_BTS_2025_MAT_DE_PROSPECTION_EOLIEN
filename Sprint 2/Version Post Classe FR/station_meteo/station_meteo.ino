#include "Anemometre.h"
#include "Girouette.h"

Anemometre anemometre(13);
Girouette girouette(12);
unsigned long dernierTempsAffichage = 0;

void setup() {
    anemometre.initialiser();
    girouette.initialiser();
}

void loop() {
    anemometre.mettreAJour();
    unsigned long tempsActuel = millis();
    
    if (tempsActuel - dernierTempsAffichage >= 1000) { // Toutes les 1 secondes
        Serial.print("Vitesse du vent : ");
        Serial.print(anemometre.obtenirVitesseVent());
        Serial.println(" km/h");

        Serial.print("Direction du vent : ");
        Serial.println(girouette.obtenirDirectionVent());

        Serial.println("-----------------------------------");
        dernierTempsAffichage = tempsActuel;
    }
}
