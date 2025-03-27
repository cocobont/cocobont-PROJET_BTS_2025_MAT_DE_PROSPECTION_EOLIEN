#include "Anemometre.h"

// Variable globale pour compter les impulsions du capteur de vent
volatile unsigned int impulsions = 0;

// Constructeur de la classe Anemometre
Anemometre::Anemometre(int brocheCapteur) 
    : _brocheCapteur(brocheCapteur), _nombreImpulsions(0), _dernierTemps(0) {}

// Méthode pour initialiser l'anémomètre
void Anemometre::initialiser() {
    Serial.begin(115200); // Démarre la communication série à 115200 bauds pour le débogage
    
    pinMode(_brocheCapteur, INPUT); // Configure la broche de l'anémomètre en entrée

    // Attache une interruption sur la broche de l'anémomètre
    attachInterrupt(digitalPinToInterrupt(_brocheCapteur), compterImpulsions, FALLING);

    Serial.println("Anémomètre initialisé");
}

// Fonction d'interruption pour compter les impulsions du capteur de vent
void IRAM_ATTR Anemometre::compterImpulsions() {
    impulsions++; // Incrémente le nombre d'impulsions chaque fois qu'une interruption se produit
}

// Méthode qui met à jour la vitesse du vent en comptant les impulsions
void Anemometre::mettreAJour() {
    unsigned long tempsActuel = millis(); // Obtient le temps actuel en millisecondes

    // Vérifie si 1 seconde s'est écoulée depuis la dernière mise à jour
    if (tempsActuel - _dernierTemps >= 1000) {
        _nombreImpulsions = impulsions; // Copie la valeur des impulsions comptées en 1 seconde
        impulsions = 0; // Réinitialise le compteur pour la prochaine période
        _dernierTemps = tempsActuel; // Met à jour le dernier temps enregistré
    }
}

// Méthode qui retourne la vitesse du vent en km/h
int Anemometre::obtenirVitesseVent() {
    return (_nombreImpulsions * 2.4); // Convertit le nombre d'impulsions en km/h
}
