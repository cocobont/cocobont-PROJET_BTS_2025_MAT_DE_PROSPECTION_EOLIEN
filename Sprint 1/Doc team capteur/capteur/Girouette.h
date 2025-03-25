#ifndef GIROUETTE_H
#define GIROUETTE_H

#include <Arduino.h>

class Girouette {
private:
  int _pinGirouette;
  
  // Tensions associées aux directions (en volts)
  float _tensionsDirections[8] = {
    2.44,  // N  (0°)
    1.36,  // NE (45°)
    0.17,  // E  (90°)
    0.46,  // SE (135°)
    0.79,  // S  (180°)
    1.89,  // SO (225°)
    3.15,  // O  (270°)
    2.81   // NO (315°)
  };

  // Degrés associés à chaque direction
  int _degreesDirections[8] = {0, 45, 90, 135, 180, 225, 270, 315};

  // Nom des directions
  const char* _directions[8] = {"N", "N-E", "E", "S-E", "S", "S-O", "O", "N-O"};

  // Tolérance pour la détection des directions
  float _tolerance = 0.2;

public:
  // Constructeur
  Girouette(int pin);
  
  // Fonction pour trouver l'index de la direction la plus proche
  int trouverDirectionPlusProche(float tension);
  
  // Fonction pour déterminer les deux directions les plus proches et calculer leur proportion
  void trouverDirectionsRelatives(float tension, int &dir1, int &dir2, float &pourcentage1, float &pourcentage2);
  
  // Fonction pour lire et afficher la direction du vent
  void lireDirectionVent();
  
  // Getters pour récupérer les directions et degrés
  const char* getDirectionName(int index);
  int getDirectionDegrees(int index);
};

#endif // GIROUETTE_H
