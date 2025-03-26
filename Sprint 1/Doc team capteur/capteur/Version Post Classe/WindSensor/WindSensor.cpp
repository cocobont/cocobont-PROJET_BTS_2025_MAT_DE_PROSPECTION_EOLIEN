// WindSensor.cpp
#include "WindSensor.h" // Inclusion du fichier d'en-tête pour la classe WindSensor

// Déclaration d'une variable globale pour compter les impulsions du capteur de vent.
// "volatile" signifie que cette variable peut être modifiée à tout moment par une interruption.
volatile unsigned int impulses = 0;

// Constructeur de la classe WindSensor
// Il initialise les broches et les variables internes
WindSensor::WindSensor(int pinAnemometer, int pinVane)
    : _pinAnemometer(pinAnemometer), _pinVane(pinVane), _nbImpulses(0), _lastTime(0) {}
// Ici, `: _pinAnemometer(pinAnemometer), _pinVane(pinVane), ...` est une **liste d'initialisation**
// qui assigne directement les valeurs aux attributs de la classe.


// Méthode pour initialiser le capteur de vent
void WindSensor::begin() {
    Serial.begin(115200); // Démarre la communication série à 115200 bauds pour le débogage.
    
    pinMode(_pinAnemometer, INPUT); // Configure la broche de l'anémomètre en entrée

    // Attache une interruption sur la broche de l'anémomètre.
    // Chaque fois qu'un front descendant (FALLING) est détecté, la fonction countImpulses() est appelée.
    attachInterrupt(digitalPinToInterrupt(_pinAnemometer), countImpulses, FALLING);

    // Affichage de messages sur le moniteur série
    Serial.println("Station météo initialisée");
    Serial.println("------------------------");
}


// Fonction d'interruption pour compter les impulsions du capteur de vent
void IRAM_ATTR WindSensor::countImpulses() {
    impulses++; // Incrémente le nombre d'impulsions chaque fois qu'une interruption se produit.
}

// Méthode qui met à jour la vitesse du vent en comptant les impulsions
void WindSensor::update() {
    unsigned long currentTime = millis(); // Obtient le temps actuel en millisecondes

    // Vérifie si 1 seconde s'est écoulée depuis la dernière mise à jour
    if (currentTime - _lastTime >= 1000) {
        _nbImpulses = impulses; // Copie la valeur des impulsions comptées en 1 seconde
        impulses = 0; // Réinitialise le compteur pour la prochaine période
        _lastTime = currentTime; // Met à jour le dernier temps enregistré
    }
}

// Méthode qui retourne la vitesse du vent en km/h
int WindSensor::getWindSpeed() {
    return (_nbImpulses * 2.4); // Convertit le nombre d'impulsions en km/h
}
// Explication :
// - La formule est spécifique au capteur utilisé (dépend du nombre d'impulsions par tour).
// - Ici, chaque impulsion représente une fraction de la vitesse réelle du vent.


// Méthode qui retourne la direction du vent sous forme de chaîne de caractères
const char* WindSensor::getWindDirection() {
    int analogValue = analogRead(_pinVane); // Lit la valeur analogique de la girouette (0-4095)
    float tension = analogValue * (3.3 / 4095); // Convertit cette valeur en tension (de 0V à 3.3V)

    // Trouve l'indice de la direction la plus proche en comparant la tension mesurée aux valeurs de référence
    int index = findClosestDirection(tension);

    // Retourne la direction correspondante (ex: "Nord", "Sud-Ouest", etc.)
    return index >= 0 ? _directions[index] : "Inconnue";
}


// Méthode privée qui trouve la direction du vent la plus proche
int WindSensor::findClosestDirection(float tension) {
    int closestIndex = -1; // Index de la meilleure correspondance (-1 si non trouvé)
    float minDifference = 5.0; // Initialise la différence minimale avec une valeur élevée

    // Parcourt les 8 valeurs de tension associées aux directions de la girouette
    for (int i = 0; i < 8; i++) {
        float difference = abs(tension - _tensionDirections[i]); // Calcule la différence entre la tension mesurée et la tension de référence

        // Si cette différence est plus petite que la précédente ET inférieure à la tolérance définie
        if (difference < minDifference && difference < _tolerance) {
            minDifference = difference; // Met à jour la différence minimale
            closestIndex = i; // Stocke l'index de la meilleure correspondance
        }
    }

    return closestIndex; // Retourne l'index de la direction la plus proche
}
