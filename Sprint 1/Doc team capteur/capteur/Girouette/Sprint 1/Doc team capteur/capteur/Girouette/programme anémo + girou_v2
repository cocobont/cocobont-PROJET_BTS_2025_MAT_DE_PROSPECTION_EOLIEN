const int pinAnemometre = 13;  // Broche numérique pour l'anémomètre
const int pinGirouette = 12;   // Broche analogique pour la girouette

volatile unsigned int nbImpulsions = 0;  // Compteur d'impulsions pour l'anémomètre
unsigned long dernierTemps = 0;  // Dernier temps pour le calcul de la vitesse du vent
float vitesseVent = 0;

// Plages de tensions associées aux directions (en volts)
float tensionsDirections[][2] = {
  {2.39, 2.49},  // N (~2.39V)
  {1.26, 1.46},  // NE (~1.36V)
  {0.07, 0.27},  // E (~0.17V)
  {0.36, 0.56},  // SE (~0.46V)
  {0.69, 0.89},  // S (~0.79V)
  {1.69, 2.09},  // SW (~1.89V)
  {3.05, 3.25},  // W (~3.15V)
  {2.71, 2.91}   // NW (~2.81V)
};

// Nom des directions
const char* directions[] = {
  "N", "N-E", "E", "S-E", "S", "S-O", "O", "N-O"
};

// Tolérance ajustée pour éviter les chevauchements
float tolerance = 0.1;

void setup() {
  Serial.begin(115200);
  pinMode(pinAnemometre, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinAnemometre), compteurImpulsions, FALLING);
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

// Fonction pour lire et afficher la direction du vent
void lireDirectionVent() {
  int valeurNumerique = analogRead(pinGirouette);
  float tensionLue = valeurNumerique * (3.3 / 4095);
  Serial.print("Tension lue : ");
  Serial.println(tensionLue, 4);
  
  int direction = -1;

  // Check in order from highest voltage to lowest
  if (tensionLue >= 3.05 && tensionLue <= 3.25) {
    direction = 6; // W (Ouest)
  } else if (tensionLue >= 2.71 && tensionLue <= 2.91) {
    direction = 7; // NW (Nord-Ouest)
  } else if (tensionLue >= 2.39 && tensionLue <= 2.49) {
    direction = 0; // N (Nord)
  } else if (tensionLue >= 1.69 && tensionLue <= 2.09) {
    direction = 5; // SW (Sud-Ouest)
  } else if (tensionLue >= 1.26 && tensionLue <= 1.46) {
    direction = 1; // NE (Nord-Est)
  } else if (tensionLue >= 0.69 && tensionLue <= 0.89) {
    direction = 4; // S (Sud)
  } else if (tensionLue >= 0.39 && tensionLue <= 0.56) {
    direction = 3; // SE (Sud-Est)
  } else if (tensionLue >= 0.07 && tensionLue <= 0.27) {
    direction = 2; // E (Est)
  }

  Serial.print("Direction : ");
  if (direction >= 0) {
    Serial.println(directions[direction]);
  } else {
    Serial.println("Inconnue");
  }
}
