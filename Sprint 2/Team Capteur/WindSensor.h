// WindSensor.h
#ifndef WIND_SENSOR_H
#define WIND_SENSOR_H

#include <Arduino.h> // Inclusion de la bibliothèque Arduino pour utiliser ses fonctions

class WindSensor {
public:
    // Constructeur : initialise le capteur avec les broches de l'anémomètre et de la girouette
    WindSensor(int pinAnemometer, int pinVane);

    // Initialise le capteur (configuration des broches, interruption, etc.)
    void begin();

    // Met à jour les valeurs des capteurs (doit être appelé régulièrement)
    void update();

    // Retourne la vitesse du vent en fonction des impulsions
    int getWindSpeed();

    // Retourne la direction du vent sous forme de chaîne de caractères
    const char* getWindDirection();

private:
    int _pinAnemometer; // Broche reliée à l'anémomètre
    int _pinVane;       // Broche reliée à la girouette

    volatile unsigned int _nbImpulses; // Nombre d'impulsions comptées par l'anémomètre
    unsigned long _lastTime;           // Dernier moment où l'anémomètre a été lu

    // Tableau des tensions de sortie de la girouette pour différentes directions du vent
    float _tensionDirections[8] = {2.44, 1.36, 0.17, 0.46, 0.79, 1.89, 3.15, 2.81};

    // Tableau des angles correspondants aux tensions ci-dessus
    int _degreesDirections[8] = {0, 45, 90, 135, 180, 225, 270, 315};

    // Tableau des directions du vent sous forme de chaîne de caractères
    const char* _directions[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};

    float _tolerance = 0.2; // Marge d'erreur tolérée pour comparer une tension

    // Fonction d'interruption appelée quand l'anémomètre génère une impulsion
    static void IRAM_ATTR countImpulses(); //Instruction RAM ATTRIbute
    //Résumé rapide
    //IRAM_ATTR → Stocke la fonction en RAM pour des interruptions plus rapides et stables.
    //Essentiel pour les ISR sur ESP32 car la mémoire Flash est plus lente.
    //Si absent, risque de latence et crash si la Flash n'est pas accessible.

    // Trouve la direction du vent la plus proche en fonction de la tension lue
    int findClosestDirection(float tension);
};
//Pourquoi y a-t-il des _ avant certaines variables ?
//Les underscore (soulignés) sont une convention pour indiquer que ces variables sont privées de la classe.
//Cela permet de différencier clairement les variables internes de celles accessibles par l'utilisateur de la classe.
#endif
