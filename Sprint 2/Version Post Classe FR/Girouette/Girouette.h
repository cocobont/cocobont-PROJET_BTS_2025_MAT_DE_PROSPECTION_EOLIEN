#ifndef GIROUETTE_H
#define GIROUETTE_H

#include <Arduino.h>

class Girouette {
public:
    // Constructeur : initialise la girouette avec sa broche
    Girouette(int brocheCapteur);

    // Initialise le capteur
    void initialiser();

    // Retourne la direction du vent sous forme de chaîne de caractères
    const char* obtenirDirectionVent();

private:
    int _brocheCapteur;  // Broche reliée à la girouette

    // Tableau des tensions de sortie de la girouette pour différentes directions du vent
    float _tensionsDirections[8] = {2.44, 1.36, 0.17, 0.46, 0.79, 1.89, 3.15, 2.81};

    // Tableau des directions du vent sous forme de chaîne de caractères
    const char* _directions[8] = {"Nord", "Nord-Est", "Est", "Sud-Est", "Sud", "Sud-Ouest", "Ouest", "Nord-Ouest"};

    float _toleranceErreur = 0.2; // Marge d'erreur tolérée pour comparer une tension

    // Trouve la direction du vent la plus proche en fonction de la tension lue
    int trouverDirectionPlusProche(float tension);
};

#endif
