#ifndef ANEMOMETRE_H
#define ANEMOMETRE_H

#include <Arduino.h>

class Anemometre {
public:
    // Constructeur : initialise l'anémomètre avec sa broche
    Anemometre(int brocheCapteur);

    // Initialise le capteur (configuration des broches, interruption, etc.)
    void initialiser();

    // Met à jour les valeurs du capteur (doit être appelé régulièrement)
    void mettreAJour();

    // Retourne la vitesse du vent en km/h
    int obtenirVitesseVent();

private:
    int _brocheCapteur;      // Broche reliée à l'anémomètre
    volatile unsigned int _nombreImpulsions; // Nombre d'impulsions comptées
    unsigned long _dernierTemps;  // Dernier moment où l'anémomètre a été lu

    // Fonction d'interruption appelée quand l'anémomètre génère une impulsion
    static void IRAM_ATTR compterImpulsions();
};

#endif
