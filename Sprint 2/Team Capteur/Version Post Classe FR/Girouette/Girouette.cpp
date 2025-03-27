#include "Girouette.h"

// Constructeur de la classe Girouette
Girouette::Girouette(int brocheCapteur) 
    : _brocheCapteur(brocheCapteur) {}

// Méthode pour initialiser la girouette
void Girouette::initialiser() {
    pinMode(_brocheCapteur, INPUT); // Configure la broche de la girouette en entrée
    Serial.println("Girouette initialisée");
}

// Méthode qui retourne la direction du vent sous forme de chaîne de caractères
const char* Girouette::obtenirDirectionVent() {
    int valeurAnalogique = analogRead(_brocheCapteur); // Lit la valeur analogique de la girouette (0-4095)
    float tension = valeurAnalogique * (3.3 / 4095); // Convertit cette valeur en tension (de 0V à 3.3V)

    // Trouve l'indice de la direction la plus proche en comparant la tension mesurée aux valeurs de référence
    int indice = trouverDirectionPlusProche(tension);

    // Retourne la direction correspondante
    return indice >= 0 ? _directions[indice] : "Inconnue";
}

// Méthode privée qui trouve la direction du vent la plus proche
int Girouette::trouverDirectionPlusProche(float tension) {
    int indiceMeilleure = -1; // Index de la meilleure correspondance (-1 si non trouvé)
    float differenceMinimale = 5.0; // Initialise la différence minimale avec une valeur élevée

    // Parcourt les 8 valeurs de tension associées aux directions de la girouette
    for (int i = 0; i < 8; i++) {
        float difference = abs(tension - _tensionsDirections[i]); // Calcule la différence entre la tension mesurée et la tension de référence

        // Si cette différence est plus petite que la précédente ET inférieure à la tolérance définie
        if (difference < differenceMinimale && difference < _toleranceErreur) {
            differenceMinimale = difference; // Met à jour la différence minimale
            indiceMeilleure = i; // Stocke l'index de la meilleure correspondance
        }
    }

    return indiceMeilleure; // Retourne l'index de la direction la plus proche
}
