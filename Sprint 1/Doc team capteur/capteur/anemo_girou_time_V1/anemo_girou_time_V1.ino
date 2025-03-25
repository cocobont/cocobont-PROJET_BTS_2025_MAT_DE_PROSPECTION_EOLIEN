#include <ESP32Time.h>  // Bibliothèque pour gérer l'horloge temps réel sur ESP32

// Création d'une instance de l'horloge temps réel avec un décalage de 0 seconde
ESP32Time rtc(0);  // Pas de décalage pour éviter les erreurs de temps

// Définition des broches pour les capteurs
const int pinAnemometre = 13;  // Broche numérique pour l'anémomètre (capteur de vitesse du vent)
const int pinGirouette = 12;   // Broche analogique pour la girouette (capteur de direction du vent)

// Variables pour le calcul de la vitesse du vent
volatile unsigned int nbImpulsions = 0;  // Compteur d'impulsions de l'anémomètre (volatile car modifié dans une interruption)
unsigned long dernierTempsVent = 0;      // Horodatage de la dernière mesure de vent
unsigned long dernierTempsAcquisition = 0; // Horodatage de la dernière acquisition complète
float vitesseVent = 0;                   // Stockage de la vitesse du vent en km/h
bool horloge_reglee = false;             // Flag indiquant si l'horloge a été réglée par l'utilisateur

// Table de correspondance entre les tensions mesurées et les directions du vent
// La girouette fournit différentes tensions selon sa position
float tensionsDirections[] = {
  2.44,  // N  (0°) - Nord
  1.36,  // NE (45°) - Nord-Est
  0.17,  // E  (90°) - Est
  0.46,  // SE (135°) - Sud-Est
  0.79,  // S  (180°) - Sud
  1.89,  // SO (225°) - Sud-Ouest
  3.15,  // O  (270°) - Ouest
  2.81   // NO (315°) - Nord-Ouest
};

// Valeurs en degrés correspondant à chaque direction
int degreesDirections[] = {0, 45, 90, 135, 180, 225, 270, 315};

// Noms des directions en format texte
const char* directions[] = {"N", "N-E", "E", "S-E", "S", "S-O", "O", "N-O"};

// Marge d'erreur acceptable pour la détection des directions
float tolerance = 0.2;  // En volts

/**
 * Configuration initiale du système
 */
void setup() {
  Serial.begin(115200);  // Initialisation de la communication série à 115200 bauds
  delay(2000);           // Délai pour permettre l'ouverture du moniteur série
  
  // Configuration de la broche de l'anémomètre comme entrée
  pinMode(pinAnemometre, INPUT);
  
  // Configuration de l'interruption sur la broche de l'anémomètre
  // À chaque front descendant (FALLING), la fonction compteurImpulsions sera appelée
  attachInterrupt(digitalPinToInterrupt(pinAnemometre), compteurImpulsions, FALLING);
  
  // Affichage des messages d'initialisation
  Serial.println("Configuration de la station meteo");
  delay(100);
  Serial.println("Entrez la date et l'heure sous le format JJ/MM/AAAA HH:MM:SS");
  delay(100);
}

/**
 * Boucle principale du programme
 */
void loop() {
  // Vérification si l'horloge a été réglée
  if (!horloge_reglee) {
    // Si non réglée, attendre les données de l'utilisateur
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');  // Lire l'entrée jusqu'au retour à la ligne
      input.trim();  // Supprimer les espaces en début et fin
      
      // Variables pour stocker la date et l'heure
      int day, month, year, hour, minute, second;
      
      // Analyser la chaîne d'entrée pour extraire la date et l'heure
      if (sscanf(input.c_str(), "%d/%d/%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second) == 6) {
        // Si le format est correct, régler l'horloge
        rtc.setTime(second, minute, hour, day, month, year);
        Serial.println("Horloge reglee !");
        delay(100);
        Serial.println("Station meteo initialisee");
        delay(100);
        Serial.println("------------------------");
        horloge_reglee = true;  // Marquer l'horloge comme réglée
        
        // Initialiser les compteurs de temps
        dernierTempsVent = millis();
        dernierTempsAcquisition = millis();
      } else {
        // Si le format est incorrect, afficher un message d'erreur
        Serial.println("Format invalide. Reessayez (JJ/MM/AAAA HH:MM:SS)");
      }
    }
    return;  // Sortir de la fonction loop si l'horloge n'est pas réglée
  }
  
  // À ce stade, l'horloge est réglée et on peut collecter les données
  unsigned long tempsActuel = millis();  // Obtenir le temps écoulé depuis le démarrage
  
  // Calcul de la vitesse du vent toutes les secondes
  if (tempsActuel - dernierTempsVent >= 1000) {  // Si 1 seconde s'est écoulée
    // Calcul de la vitesse du vent basé sur le nombre d'impulsions
    // Facteur 2.4 pour convertir les impulsions en km/h selon les spécifications de l'anémomètre
    vitesseVent = (nbImpulsions / 1.0) * 2.4;
    nbImpulsions = 0;  // Réinitialiser le compteur d'impulsions
    dernierTempsVent = tempsActuel;  // Mettre à jour le temps de la dernière mesure
  }
  
  // Acquisition et affichage des données toutes les 2 minutes
  if (tempsActuel - dernierTempsAcquisition >= 120000) {  // 2 minutes en millisecondes
    // Affichage séparateur
    Serial.println("------------------------");
    delay(50);
    
    // Affichage de l'horodatage
    Serial.print("Horodatage: ");
    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  // Format complet date et heure
    delay(50);
    
    // Lecture et affichage de la direction du vent
    lireDirectionVent();
    
    // Affichage de la vitesse du vent
    Serial.print("Vitesse du vent : ");
    Serial.print(vitesseVent);
    Serial.println(" km/h");
    delay(50);
    
    // Mise à jour du temps de la dernière acquisition
    dernierTempsAcquisition = tempsActuel;
    Serial.println("------------------------");
  }
  
  // Gestion des commandes utilisateur
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');  // Lire la commande
    command.trim();  // Supprimer les espaces
    
    // Si la commande est "data", afficher les données immédiatement
    if (command.equals("data")) {
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

/**
 * Fonction d'interruption appelée à chaque impulsion de l'anémomètre
 * Cette fonction est exécutée à chaque front descendant détecté sur la broche de l'anémomètre
 */
void compteurImpulsions() {
  nbImpulsions++;  // Incrémente le compteur d'impulsions
}

/**
 * Fonction pour trouver l'index de la direction la plus proche dans le tableau
 * @param tension La tension mesurée sur la girouette
 * @return L'index de la direction la plus proche, ou -1 si aucune direction ne correspond
 */
int trouverDirectionPlusProche(float tension) {
  int indexPlusProche = -1;
  float differenceMin = 5.0;  // Valeur initiale supérieure à toute différence possible
  
  // Parcourir toutes les directions connues
  for (int i = 0; i < 8; i++) {
    // Calculer la différence entre la tension mesurée et la tension de référence
    float difference = abs(tension - tensionsDirections[i]);
    
    // Si cette différence est inférieure à la plus petite différence trouvée jusqu'à présent
    // et qu'elle est dans la marge de tolérance
    if (difference < differenceMin && difference < tolerance) {
      differenceMin = difference;
      indexPlusProche = i;
    }
  }
  
  return indexPlusProche;  // Retourner l'index de la direction la plus proche
}

/**
 * Fonction pour déterminer les deux directions les plus proches et calculer leur proportion
 * Cela permet d'avoir une direction plus précise entre deux points cardinaux
 * @param tension La tension mesur
 ée sur la girouette
 * @param dir1 Variable pour stocker l'index de la première direction
 * @param dir2 Variable pour stocker l'index de la deuxième direction
 * @param pourcentage1 Variable pour stocker le pourcentage de la première direction
 * @param pourcentage2 Variable pour stocker le pourcentage de la deuxième direction
 */
void trouverDirectionsRelatives(float tension, int &dir1, int &dir2, float &pourcentage1, float &pourcentage2) {
  // Initialiser avec des valeurs par défaut
  dir1 = -1;
  dir2 = -1;
  pourcentage1 = 0;
  pourcentage2 = 0;
  
  // Variables pour stocker les différences
  float diff1 = 5.0;  // Différence pour la première direction
  float diff2 = 5.0;  // Différence pour la deuxième direction
  
  // Parcourir toutes les directions connues
  for (int i = 0; i < 8; i++) {
    // Calculer la différence entre la tension mesurée et la tension de référence
    float diff = abs(tension - tensionsDirections[i]);
    
    // Si cette différence est inférieure à la plus petite différence trouvée
    if (diff < diff1) {
      // Déplacer la première différence vers la deuxième
      diff2 = diff1;
      dir2 = dir1;
      // Stocker la nouvelle plus petite différence
      diff1 = diff;
      dir1 = i;
    } else if (diff < diff2) {
      // Si elle est inférieure à la deuxième plus petite différence
      diff2 = diff;
      dir2 = i;
    }
  }
  
  // Si on n'a pas trouvé deux directions valides
  if (dir1 < 0 || dir2 < 0) {
    // Utiliser la direction la plus proche uniquement
    dir1 = trouverDirectionPlusProche(tension);
    dir2 = dir1;
    pourcentage1 = 100;
    pourcentage2 = 0;
    return;
  }
  
  // Calculer les pourcentages pour chaque direction
  float diffTotale = diff1 + diff2;
  if (diffTotale == 0) {
    // Si les différences sont nulles, utiliser uniquement la première direction
    pourcentage1 = 100;
    pourcentage2 = 0;
  } else {
    // Calculer les pourcentages inversés (plus petite différence = plus grand pourcentage)
    pourcentage1 = (1 - (diff1 / diffTotale)) * 100;
    pourcentage2 = (1 - (diff2 / diffTotale)) * 100;
    
    // Normaliser pour que la somme des pourcentages soit 100%
    float sommePourcentages = pourcentage1 + pourcentage2;
    pourcentage1 = (pourcentage1 / sommePourcentages) * 100;
    pourcentage2 = (pourcentage2 / sommePourcentages) * 100;
  }
}

/**
 * Fonction pour lire et afficher la direction du vent avec précision
 * Utilise la girouette pour déterminer la direction
 */
void lireDirectionVent() {
  // Lire la valeur analogique de la girouette (0-4095 pour ESP32)
  int valeurNumerique = analogRead(pinGirouette);
  
  // Convertir la valeur en tension (0-3.3V pour ESP32)
  float tensionLue = valeurNumerique * (3.3 / 4095);
  
  // Afficher la tension mesurée
  Serial.print("Tension lue : ");
  Serial.println(tensionLue, 4);  // 4 décimales pour la précision
  delay(50);
  
  // Trouver la direction la plus proche dans le tableau
  int indexDirection = trouverDirectionPlusProche(tensionLue);
  
  if (indexDirection >= 0) {
    // Si une direction a été trouvée, afficher son nom et sa valeur en degrés
    int degres = degreesDirections[indexDirection];
    Serial.print("Direction principale : ");
    Serial.print(directions[indexDirection]);
    Serial.print(" (");
    Serial.print(degres);
    Serial.println("°)");
    delay(50);
    
    // Calculer les directions relatives pour une mesure plus précise
    int dir1, dir2;
    float pourcentage1, pourcentage2;
    trouverDirectionsRelatives(tensionLue, dir1, dir2, pourcentage1, pourcentage2);
    
    // Si les deux directions sont différentes, afficher la composition
    if (dir1 != dir2) {
      Serial.print("Composition : ");
      Serial.print(round(pourcentage1));  // Arrondir le pourcentage
      Serial.print("% ");
      Serial.print(directions[dir1]);
      Serial.print(" + ");
      Serial.print(round(pourcentage2));  // Arrondir le pourcentage
      Serial.print("% ");
      Serial.println(directions[dir2]);
      delay(50);
      
      // Calculer une direction en degrés plus précise en tenant compte des deux directions
      float degresPrecis;
      
      // Gérer le cas spécial autour de Nord (0/360°)
      // Ce cas est particulier car on passe de 315° à 0° (ou 360°)
      if ((dir1 == 0 && dir2 == 7) || (dir1 == 7 && dir2 == 0)) {
        // Si on est entre NO (315°) et N (0°/360°)
        if (dir1 == 0) {
          // Si la première direction est Nord (0°)
          // On calcule en considérant que dir2 (NO, 315°) est en fait à -45° par rapport à Nord
          degresPrecis = (pourcentage1 * degreesDirections[dir1] + pourcentage2 * (degreesDirections[dir2] - 360)) / 100;
          // Si le résultat est négatif, ajouter 360 pour revenir dans l'intervalle [0-360]
          if (degresPrecis < 0) degresPrecis += 360;
        } else {
          // Si la première direction est NO (315°)
          // On calcule en considérant que dir2 (N, 0°) est en fait à 360° (pas à 0°)
          degresPrecis = (pourcentage1 * degreesDirections[dir1] + pourcentage2 * (degreesDirections[dir2] + 360)) / 100;
          // Si le résultat est ≥ 360, soustraire 360 pour revenir dans l'intervalle [0-360]
          if (degresPrecis >= 360) degresPrecis -= 360;
        }
      } else {
        // Cas normal pour les autres directions
        // Moyenne pondérée des deux directions en fonction de leurs pourcentages
        degresPrecis = (pourcentage1 * degreesDirections[dir1] + pourcentage2 * degreesDirections[dir2]) / 100;
      }
      
      // Afficher la direction précise calculée
      Serial.print("Direction precise : ");
      Serial.print(degresPrecis, 1);  // 1 décimale pour la précision
      Serial.println("°");
      delay(50);
    }
  } else {
    // Si aucune direction ne correspond à la tension mesurée (en dehors des tolérances)
    Serial.println("Direction : Inconnue");
    delay(50);
  }
}