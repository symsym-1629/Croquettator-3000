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
    database = SD.open("test.txt");
    if (database) {
        Serial.println("test.txt:");

        // read from the file until there's nothing else in it:
        while (database.available()) {
            Serial.write(database.read());
        }
        // close the file:
        database.close();
    } else {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
    }
}

void loop() {
    if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
            for (int i = 0; i < 5; i++) {
                Serial.print(rfid.serNum[0]);
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

