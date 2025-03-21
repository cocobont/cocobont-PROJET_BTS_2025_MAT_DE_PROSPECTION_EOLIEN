#include "Girouette.h"

// Constructeur
Girouette::Girouette(int pin) : _pinGirouette(pin) {
  pinMode(_pinGirouette, INPUT);
}

// Fonction pour trouver l'index de la direction la plus proche
int Girouette::trouverDirectionPlusProche(float tension) {
  int indexPlusProche = -1;
  float differenceMin = 5.0;  // Valeur initiale supérieure à toute différence possible
  
  for (int i = 0; i < 8; i++) {
    float difference = abs(tension - _tensionsDirections[i]);
    if (difference < differenceMin && difference < _tolerance) {
      differenceMin = difference;
      indexPlusProche = i;
    }
  }
  
  return indexPlusProche;
}

// Fonction pour déterminer les deux directions les plus proches et calculer leur proportion
void Girouette::trouverDirectionsRelatives(float tension, int &dir1, int &dir2, float &pourcentage1, float &pourcentage2) {
  // Initialiser avec des valeurs par défaut
  dir1 = -1;
  dir2 = -1;
  pourcentage1 = 0;
  pourcentage2 = 0;
  
  // Trouver les deux tensions les plus proches
  float diff1 = 5.0;
  float diff2 = 5.0;
  
  for (int i = 0; i < 8; i++) {
    float diff = abs(tension - _tensionsDirections[i]);
    
    if (diff < diff1) {
      diff2 = diff1;
      dir2 = dir1;
      diff1 = diff;
      dir1 = i;
    } else if (diff < diff2) {
      diff2 = diff;
      dir2 = i;
    }
  }
  
  // Si on n'a pas trouvé deux directions valides
  if (dir1 < 0 || dir2 < 0) {
    dir1 = trouverDirectionPlusProche(tension);
    dir2 = dir1;
    pourcentage1 = 100;
    pourcentage2 = 0;
    return;
  }
  
  // Calculer les pourcentages
  float diffTotale = diff1 + diff2;
  if (diffTotale == 0) {
    pourcentage1 = 100;
    pourcentage2 = 0;
  } else {
    pourcentage1 = (1 - (diff1 / diffTotale)) * 100;
    pourcentage2 = (1 - (diff2 / diffTotale)) * 100;
    
    // Normaliser pour que la somme soit 100%
    float sommePourcentages = pourcentage1 + pourcentage2;
    pourcentage1 = (pourcentage1 / sommePourcentages) * 100;
    pourcentage2 = (pourcentage2 / sommePourcentages) * 100;
  }
}

// Fonction pour lire et afficher la direction du vent
void Girouette::lireDirectionVent() {
  int valeurNumerique = analogRead(_pinGirouette);
  float tensionLue = valeurNumerique * (3.3 / 4095);
  Serial.print("Tension lue : ");
  Serial.println(tensionLue, 4);
  
  // Trouver la direction la plus proche
  int indexDirection = trouverDirectionPlusProche(tensionLue);
  
  if (indexDirection >= 0) {
    // Direction exacte en degrés
    int degres = _degreesDirections[indexDirection];
    Serial.print("Direction principale : ");
    Serial.print(_directions[indexDirection]);
    Serial.print(" (");
    Serial.print(degres);
    Serial.println("°)");
    
    // Calculer les directions relatives
    int dir1, dir2;
    float pourcentage1, pourcentage2;
    trouverDirectionsRelatives(tensionLue, dir1, dir2, pourcentage1, pourcentage2);
    
    // Si les deux directions sont différentes, afficher la composition
    if (dir1 != dir2) {
      Serial.print("Composition : ");
      Serial.print(round(pourcentage1));
      Serial.print("% ");
      Serial.print(_directions[dir1]);
      Serial.print(" + ");
      Serial.print(round(pourcentage2));
      Serial.print("% ");
      Serial.println(_directions[dir2]);
      
      // Calculer une direction en degrés plus précise
      float degresPrecis;
      // Gérer le cas spécial autour de Nord (0/360°)
      if ((dir1 == 0 && dir2 == 7) || (dir1 == 7 && dir2 == 0)) {
        // Si on est entre NO (315°) et N (0°/360°)
        if (dir1 == 0) {
          degresPrecis = (pourcentage1 * _degreesDirections[dir1] + pourcentage2 * (_degreesDirections[dir2] - 360)) / 100;
          if (degresPrecis < 0) degresPrecis += 360;
        } else {
          degresPrecis = (pourcentage1 * _degreesDirections[dir1] + pourcentage2 * (_degreesDirections[dir2] + 360)) / 100;
          if (degresPrecis >= 360) degresPrecis -= 360;
        }
      } else {
        // Cas normal
        degresPrecis = (pourcentage1 * _degreesDirections[dir1] + pourcentage2 * _degreesDirections[dir2]) / 100;
      }
      
      Serial.print("Direction précise : ");
      Serial.print(degresPrecis, 1);
      Serial.println("°");
    }
  } else {
    Serial.println("Direction : Inconnue");
  }
  
  Serial.println("------------------------");
}

// Getters pour récupérer les directions et degrés
const char* Girouette::getDirectionName(int index) {
  if (index >= 0 && index < 8) {
    return _directions[index];
  }
  return "Inconnu";
}

int Girouette::getDirectionDegrees(int index) {
  if (index >= 0 && index < 8) {
    return _degreesDirections[index];
  }
  return -1;
}
