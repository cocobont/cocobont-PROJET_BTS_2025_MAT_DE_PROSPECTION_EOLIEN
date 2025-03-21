#ifndef ANEMOMETRE_H
#define ANEMOMETRE_H

#include <Arduino.h>

class Anemometre {
private:
  int _pinAnemometre;
  volatile unsigned int _nbImpulsions = 0;  // Compteur d'impulsions pour l'anémomètre
  unsigned long _dernierTemps = 0;          // Dernier temps pour le calcul de la vitesse du vent
  float _vitesseVent = 0;                   // Vitesse du vent en km/h
  float _facteurConversion = 2.4;           // Facteur de conversion des impulsions en km/h
  
  // Fonction static d'interruption à définir à l'extérieur de la classe
  static Anemometre* _instance;
  static void IRAM_ATTR _compteurImpulsionsStatic();

public:
  // Constructeur
  Anemometre(int pin);
  
  // Fonction d'interruption pour compter les impulsions de l'anémomètre
  void compteurImpulsions();
  
  // Fonction pour vérifier et calculer la vitesse du vent
  bool verifierVitesseVent();
  
  // Getter pour la vitesse du vent
  float getVitesseVent();
  
  // Afficher la vitesse du vent
  void afficherVitesseVent();
};

#endif // ANEMOMETRE_H
