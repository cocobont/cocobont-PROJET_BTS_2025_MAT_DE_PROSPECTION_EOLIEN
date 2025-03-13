const int pinGirouette = 12;  // Broche analogique connectée à la girouette

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

// Plage de tolérance pour chaque direction (en volts)
float tolerance = 0.2;  // Tolérance de 0.2V pour chaque direction

void setup() {
  Serial.begin(115200);  // Initialisation de la communication série
}

void loop() {
  // Lire la valeur de la girouette
  int valeurNumerique = analogRead(pinGirouette);  // Lecture de la valeur analogique
  float tensionLue = valeurNumerique * (3.3 / 4095);  // Convertir la valeur en tension (0-3.3V)
  
  // Affichage de la tension lue
  Serial.print("Tension lue : ");
  Serial.println(tensionLue, 4);  // Affichage avec 4 décimales pour plus de précision
  
  // Vérification de la direction en fonction des plages de tensions
  int direction = -1;
  for (int i = 0; i < 8; i++) {  // Nous avons 8 directions
    if (tensionLue >= tensionsDirections[i][0] - tolerance && tensionLue <= tensionsDirections[i][1] + tolerance) {
      direction = i;
      break;
    }
  }

  // Affichage de la direction
  if (direction >= 0) {
    Serial.print("Direction : ");
    Serial.println(directions[direction]);
  } else {
    Serial.println("Direction Inconnue");
  }
  
  delay(2500);  // Attendre 1 seconde avant la prochaine lecture
}
