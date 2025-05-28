
/*
 * Pin layout should be as follows (on Arduino Uno - Velleman VMA100):
 * //RFID Reader\\
 * MOSI: Pin 11
 * MISO: Pin 12
 * SCK: Pin 13
 * NSS: Pin 10
 * RST: Pin 9
 * VCC: 3,3V
 * GND: GND
 * IRQ: not used
 */

// déclaration des librairies (y'en a plusieurs pour les tests du RFID) 
#include <SPI.h>
#include <RFID.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <Dictionary.h>
#include <DictionaryDeclarations.h>

// Définition du capteur RFID
#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN, RST_PIN);

int power = 7;
int led = 8;
int serNum[5];
int cards[][5] = {{250, 253, 204, 106, 161}}; // Liste des cartes autorisées par numéro de série
bool access = false;

// Mise en place de la BDD
#define CS_PIN 4
File database;
JsonDocument jsonData;
Dictionary &data = *(new Dictionary());
Dictionary animals[10];


void setup() {
    Serial.begin(9600);

    // Initialisation du capteur RFID
    Serial.print("Initialisation du capteur RFID...");
    SPI.begin();
    rfid.init();
    pinMode(led, OUTPUT);
    pinMode(power, OUTPUT);
    digitalWrite(led, LOW);
    digitalWrite(power, LOW);
    Serial.println("Capteur RFID initialisé.");

    // Initialisation de la carte SD
    Serial.print("Initialisation de la carte SD...");

    if (!SD.begin(CS_PIN))
    {
        Serial.println("Carte SD non trouvée.");
        while (true);
    }

    Serial.println("Carte SD initialisée.");

    // test pour la lecture / exploitation de la BDD
    // TODO : faire fonctionner la BDD et mettre ca dans une fonction séparée
    database = SD.open("data.txt");
    if (database) {
        while (database.available())
        {

            data.jload(database);
        }

        // deserializeJson(jsonData, database);
        Serial.println(data.json());
    }
    else {
        Serial.println('ca marche paaaaas');
    }
}
 // TODO : Faire l'affichage avec l'écran LCD
void loop() {
    // lecture de la carte RFID 
    // TODO : mettre ca dans une fonction séparée
    if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
            for (int i = 0; i < 5; i++) {
                Serial.print(rfid.serNum[i]);
                Serial.print(" ");
            }
            Serial.println("");
// fonction pour vérifier si la carte est autorisée, pas touche, ca marche
            for (int x = 0; x < sizeof(cards); x++) {
                for (int i = 0; i < sizeof(rfid.serNum); i++) {
                    if (rfid.serNum[i] != cards[x][i]) {
                        access = false;
                        break;
                    }
                    else {
                        access = true;
                    }
                }
                if (access) break;
            }
        }

        if (access) {
            // carte autorisée 
            Serial.println("Carte autorisée !");
            // TODO : Récupérer les informations de la carte dans la BDD
            // TODO : Peser l'animal
            // TODO : Enregistrer le poids dans la BDD
            // TODO : Distribuer la nourriture
        }
        else {
            // carte non autorisée
            Serial.println("Carte non autorisée !");
        }
    }
    rfid.halt();
}

// Fonction pour récupérer l'ID de la carte RFID
char* getTagID() {
    char *tagID = "";
    for (int i = 0; i < 5; i++)
    {
        tagID += rfid.serNum[i];
    }
    return tagID;
}

// Fonction pour récupérer la liste des fichiers de la carte SD
char[10] getDirectoryFromDirectory(File dir) {
    int i = 0;
    char dirList[];
    dirList[0] = '\0'; // Initialize the string to be empty
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            break;
        }
        dirList[i] = entry.name();
        entry.close();
        i++;
    }
    return dirList;
}
