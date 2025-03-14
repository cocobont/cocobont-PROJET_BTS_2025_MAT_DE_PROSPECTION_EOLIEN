const int pinAnemometre = 13;  // Broche numérique pour l'anémomètre
const int pinGirouette = 12;   // Broche analogique pour la girouette
volatile unsigned int nbImpulsions = 0;  // Compteur d'impulsions pour l'anémomètre
unsigned long dernierTemps = 0;          // Dernier temps pour le calcul de la vitesse du vent
float vitesseVent = 0;                   // Vitesse du vent en km/h

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
  pinMode(pinAnemometre, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinAnemometre), compteurImpulsions, FALLING);
  
  Serial.println("Station météo initialisée");
  Serial.println("------------------------");
}

void loop() {
  unsigned long tempsActuel = millis();
  
  // Lecture et affichage de la direction du vent
  lireDirectionVent();
  
  // Vérification toutes les secondes pour la vitesse du vent
  if (tempsActuel - dernierTemps >= 1000) {
    vitesseVent = (nbImpulsions / 1.0) * 2.4;
    Serial.print("Vitesse du vent : ");
    Serial.print(vitesseVent);
    Serial.println(" km/h");
    nbImpulsions = 0;
    dernierTemps = tempsActuel;
  }
  
  delay(2500);  // Pause avant la prochaine lecture
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
      
      Serial.print("Direction précise : ");
      Serial.print(degresPrecis, 1);
      Serial.println("°");
    }
  } else {
    Serial.println("Direction : Inconnue");
  }
  
  Serial.println("------------------------");
}