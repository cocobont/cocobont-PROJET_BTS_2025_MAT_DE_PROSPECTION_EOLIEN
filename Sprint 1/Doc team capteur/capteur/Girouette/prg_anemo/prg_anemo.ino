const int pinAnemometre = 13;  // Broche numérique connectée à l'anémomètre
volatile unsigned int nbImpulsions = 0;  // Compteur d'impulsions
unsigned long dernierTemps = 0;  // Dernier temps pour le calcul de la vitesse
float vitesseVent = 0;

void setup() {
  Serial.begin(115200);  // Initialisation de la communication série
  pinMode(pinAnemometre, INPUT);  // Configuration de la broche en entrée
  attachInterrupt(digitalPinToInterrupt(pinAnemometre), compteurImpulsions, FALLING);  // Interruption sur front descendant
}

void loop() {
  unsigned long tempsActuel = millis();  // Temps actuel en millisecondes

  // Vérification toutes les secondes
  if (tempsActuel - dernierTemps >= 1000) {  // Si une seconde est passée
    // Calcul de la vitesse du vent (1 impulsion = 2.4 km/h, ajuster si nécessaire)
    vitesseVent = (nbImpulsions / 1.0) * 2.4;

    // Affichage de la vitesse du vent
    Serial.print("Vitesse du vent : ");
    Serial.print(vitesseVent);
    Serial.println(" km/h");

    // Réinitialiser le compteur d'impulsions et mettre à jour le temps
    nbImpulsions = 0;  // Réinitialisation après chaque seconde
    dernierTemps = tempsActuel;  // Mise à jour du temps

    // Note : les impulsions ne sont comptées que pendant cette période de 1 seconde
  }

  delay(100);  // Petite pause pour éviter un trop grand nombre de lectures
}

// Fonction d'interruption pour compter les impulsions
void compteurImpulsions() {
  nbImpulsions++;  // Incrémenter le nombre d'impulsions à chaque changement d'état
}
