const int pinGirouette = 12;  // Broche analogique connectée à la girouette

// Plages de tensions associées aux directions (en volts)
float tensionsDirections[][2] = {
  {0.0, 0.5},   // N (0°) : 0V à 0.5V
  {0.5, 1.0},   // N-E (45°) : 0.5V à 1.0V
  {1.0, 1.5},   // E (90°) : 1.0V à 1.5V
  {1.5, 2.0},   // S-E (135°) : 1.5V à 2.0V
  {2.0, 2.5},   // S (180°) : 2.0V à 2.5V
  {2.5, 3.0},   // S-O (225°) : 2.5V à 3.0V
  {3.0, 3.3},   // O (270°) : 3.0V à 3.3V
  {0.0, 0.1}    // N-O (315°) : 0.0V à 0.1V
};

// Nom des directions
const char* directions[] = {
  "N", "N-E", "E", "S-E", "S", "S-O", "O", "N-O"
};

// Plage de tolérance pour chaque direction (en volts)
float tolerance = 0.2;  // Tolérance de 0.2V pour chaque direction

void setup() {
  Serial.begin(9600);  // Initialisation de la communication série
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
  
  delay(1000);  // Attendre 1 seconde avant la prochaine lecture
}
