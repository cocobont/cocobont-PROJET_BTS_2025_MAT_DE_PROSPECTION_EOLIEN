#include "Anemometre.h"

// Initialisation de la variable statique
Anemometre* Anemometre::_instance = nullptr;

// Fonction statique d'interruption
void IRAM_ATTR Anemometre::_compteurImpulsionsStatic() {
  if (_instance)
    _instance->compteurImpulsions();
}

// Constructeur
Anemometre::Anemometre(int pin) : _pinAnemometre(pin) {
  pinMode(_pinAnemometre, INPUT);
  _instance = this;
  attachInterrupt(digitalPinToInterrupt(_pinAnemometre), _compteurImpulsionsStatic, FALLING);
}

// Fonction d'interruption pour compter les impulsions de l'anémomètre
void Anemometre::compteurImpulsions() {
  _nbImpulsions++;
}

// Fonction pour vérifier et calculer la vitesse du vent
bool Anemometre::verifierVitesseVent() {
  unsigned long tempsActuel = millis();
  
  // Vérification toutes les secondes pour la vitesse du vent
  if (tempsActuel - _dernierTemps >= 1000) {
    _vitesseVent = (_nbImpulsions / 1.0) * _facteurConversion;
    _nbImpulsions = 0;
    _dernierTemps = tempsActuel;
    return true;
  }
  return false;
}

// Getter pour la vitesse du vent
float Anemometre::getVitesseVent() {
  return _vitesseVent;
}

// Afficher la vitesse du vent
void Anemometre::afficherVitesseVent() {
  Serial.print("Vitesse du vent : ");
  Serial.print(_vitesseVent);
  Serial.println(" km/h");
}
