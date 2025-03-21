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

#include <SPI.h>
#include <RFID.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

// Définition du capteur RFID
#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN, RST_PIN);

int power = 7;
int led = 8;
int serNum[5];
int cards[][5] = {{250, 253, 204, 106, 161}};
bool access = false;

// Mise en place de la BDD
#define CS_PIN 4
File database;
JsonDocument jsonData;

/*
 * This integer should be the code of Your Mifare card / tag
 */

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

    if (!SD.begin(CS_PIN)) {
        Serial.println("Carte SD non trouvée.");
        while (true);
    }

    Serial.println("Carte SD initialisée.");

    // test 
    database = SD.open("data.json");
    if (database) {
        while (database.available()) {
            Serial.write(database.read());
    }
    } else {
        Serial.println('ca marche paaaaas');
    }
}

void loop() {
    if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
            for (int i = 0; i < 5; i++) {
                Serial.print(rfid.serNum[i]);
                Serial.print(" ");
            }
            Serial.println("");

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
            Serial.println("Welcome Velleman ");
            /*
             * Valid card : Switch ON the LED for 1000 ms (1 second)
             */
            digitalWrite(led, HIGH);
            delay(1000);
            /*
             * Valid card : Switch ON the POWER PIN for 2000 ms (2 seconds)), the POWER PIN can activate for example a relais, controlling a doorlock)
             */
            digitalWrite(power, HIGH);
            delay(2000);
            digitalWrite(power, LOW);
            digitalWrite(led, LOW);
            access = false;
        }
        else {
            /*
             * NON-Valid card : switch ON and OFF the LED twice for 0,5 seconds
             */
            Serial.println("Not allowed!");
            digitalWrite(led, HIGH);
            delay(500);
            digitalWrite(led, LOW);
            delay(500);
            digitalWrite(led, HIGH);
            delay(500);
            digitalWrite(led, LOW);
        }
    }

    rfid.halt();
}

char* getTagID() {
    char* tagID = "";
    for (int i = 0; i < 5; i++) {
        tagID += rfid.serNum[i];
    }
    return tagID;
}

