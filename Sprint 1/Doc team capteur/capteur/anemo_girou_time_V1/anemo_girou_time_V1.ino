#include <ESP32Time.h>

ESP32Time rtc(0);  // Pas de décalage pour éviter les erreurs

const int pinAnemometre = 13;  // Broche numérique pour l'anémomètre
const int pinGirouette = 12;   // Broche analogique pour la girouette
volatile unsigned int nbImpulsions = 0;  // Compteur d'impulsions pour l'anémomètre
unsigned long dernierTempsVent = 0;      // Dernier temps pour le calcul de la vitesse du vent
unsigned long dernierTempsAcquisition = 0; // Pour l'acquisition toutes les 2 minutes
float vitesseVent = 0;                   // Vitesse du vent en km/h
bool horloge_reglee = false;             // Flag pour savoir si l'horloge a été réglée

// Tensions associées aux directions (en volts)
float tensionsDirections[] = {
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
int degreesDirections[] = {0, 45, 90, 135, 180, 225, 270, 315};

// Nom des directions
const char* directions[] = {"N", "N-E", "E", "S-E", "S", "S-O", "O", "N-O"};

// Tolérance pour la détection des directions
float tolerance = 0.2;

void setup() {
  Serial.begin(115200);
  delay(2000); // Attendre que le port série soit prêt
  
  pinMode(pinAnemometre, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinAnemometre), compteurImpulsions, FALLING);
  
  // Corriger les messages d'initialisation avec des sauts de ligne séparés
  Serial.println("Configuration de la station meteo");
  delay(100);
  Serial.println("Entrez la date et l'heure sous le format JJ/MM/AAAA HH:MM:SS");
  delay(100);
}

void loop() {
  // Si l'horloge n'est pas encore réglée, attendre l'entrée utilisateur
  if (!horloge_reglee) {
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');
      input.trim();
      
      int day, month, year, hour, minute, second;
      if (sscanf(input.c_str(), "%d/%d/%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second) == 6) {
        rtc.setTime(second, minute, hour, day, month, year);
        Serial.println("Horloge reglee !");
        delay(100);
        Serial.println("Station meteo initialisee");
        delay(100);
        Serial.println("------------------------");
        horloge_reglee = true;
        
        // Initialiser les temps pour les acquisitions
        dernierTempsVent = millis();
        dernierTempsAcquisition = millis();
      } else {
        Serial.println("Format invalide. Reessayez (JJ/MM/AAAA HH:MM:SS)");
      }
    }
    return; // Ne pas continuer si l'horloge n'est pas réglée
  }
  
  // À partir d'ici, l'horloge est réglée et on peut collecter les données
  unsigned long tempsActuel = millis();
  
  // Vérification toutes les secondes pour la vitesse du vent
  if (tempsActuel - dernierTempsVent >= 1000) {
    vitesseVent = (nbImpulsions / 1.0) * 2.4;
    nbImpulsions = 0;
    dernierTempsVent = tempsActuel;
  }
  
  // Acquisition des données toutes les 2 minutes
  if (tempsActuel - dernierTempsAcquisition >= 120000) { // 2 minutes en millisecondes
    Serial.println("------------------------");
    delay(50);
    Serial.print("Horodatage: ");
    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
    delay(50);
    
    // Lecture et affichage de la direction du vent
    lireDirectionVent();
    
    // Affichage de la vitesse du vent
    Serial.print("Vitesse du vent : ");
    Serial.print(vitesseVent);
    Serial.println(" km/h");
    delay(50);
    
    dernierTempsAcquisition = tempsActuel;
    Serial.println("------------------------");
  }
  
  // Vérifier si des commandes utilisateur sont disponibles
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.equals("data")) {
      // Commande manuelle pour obtenir les données immédiatement
      Serial.println("------------------------");
      delay(50);
      Serial.print("Horodatage: ");
      Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
      delay(50);
      lireDirectionVent();
      Serial.print("Vitesse du vent : ");
      Serial.print(vitesseVent);
      Serial.println(" km/h");
      delay(50);
      Serial.println("------------------------");
    }
  }
}

// Fonction d'interruption pour compter les impulsions de l'anémomètre
void compteurImpulsions() {
  nbImpulsions++;
}

// Fonction pour trouver l'index de la direction la plus proche
int trouverDirectionPlusProche(float tension) {
  int indexPlusProche = -1;
  float differenceMin = 5.0;  // Valeur initiale supérieure à toute différence possible
  
  for (int i = 0; i < 8; i++) {
    float difference = abs(tension - tensionsDirections[i]);
    if (difference < differenceMin && difference < tolerance) {
      differenceMin = difference;
      indexPlusProche = i;
    }
  }
  
  return indexPlusProche;
}

// Fonction pour déterminer les deux directions les plus proches et calculer leur proportion
void trouverDirectionsRelatives(float tension, int &dir1, int &dir2, float &pourcentage1, float &pourcentage2) {
  // Initialiser avec des valeurs par défaut
  dir1 = -1;
  dir2 = -1;
  pourcentage1 = 0;
  pourcentage2 = 0;
  
  // Trouver les deux tensions les plus proches
  float diff1 = 5.0;
  float diff2 = 5.0;
  
  for (int i = 0; i < 8; i++) {
    float diff = abs(tension - tensionsDirections[i]);
    
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

// Fonction pour lire et afficher la direction du vent avec plus de précision
void lireDirectionVent() {
  int valeurNumerique = analogRead(pinGirouette);
  float tensionLue = valeurNumerique * (3.3 / 4095);
  Serial.print("Tension lue : ");
  Serial.println(tensionLue, 4);
  delay(50);
  
  // Trouver la direction la plus proche
  int indexDirection = trouverDirectionPlusProche(tensionLue);
  
  if (indexDirection >= 0) {
    // Direction exacte en degrés
    int degres = degreesDirections[indexDirection];
    Serial.print("Direction principale : ");
    Serial.print(directions[indexDirection]);
    Serial.print(" (");
    Serial.print(degres);
    Serial.println("°)");
    delay(50);
    
    // Calculer les directions relatives
    int dir1, dir2;
    float pourcentage1, pourcentage2;
    trouverDirectionsRelatives(tensionLue, dir1, dir2, pourcentage1, pourcentage2);
    
    // Si les deux directions sont différentes, afficher la composition
    if (dir1 != dir2) {
      Serial.print("Composition : ");
      Serial.print(round(pourcentage1));
      Serial.print("% ");
      Serial.print(directions[dir1]);
      Serial.print(" + ");
      Serial.print(round(pourcentage2));
      Serial.print("% ");
      Serial.println(directions[dir2]);
      delay(50);
      
      // Calculer une direction en degrés plus précise
      float degresPrecis;
      // Gérer le cas spécial autour de Nord (0/360°)
      if ((dir1 == 0 && dir2 == 7) || (dir1 == 7 && dir2 == 0)) {
        // Si on est entre NO (315°) et N (0°/360°)
        if (dir1 == 0) {
          degresPrecis = (pourcentage1 * degreesDirections[dir1] + pourcentage2 * (degreesDirections[dir2] - 360)) / 100;
          if (degresPrecis < 0) degresPrecis += 360;
        } else {
          degresPrecis = (pourcentage1 * degreesDirections[dir1] + pourcentage2 * (degreesDirections[dir2] + 360)) / 100;
          if (degresPrecis >= 360) degresPrecis -= 360;
        }
      } else {
        // Cas normal
        degresPrecis = (pourcentage1 * degreesDirections[dir1] + pourcentage2 * degreesDirections[dir2]) / 100;
      }
      
      Serial.print("Direction precise : ");
      Serial.print(degresPrecis, 1);
      Serial.println("°");
      delay(50);
    }
  } else {
    Serial.println("Direction : Inconnue");
    delay(50);
  }
}