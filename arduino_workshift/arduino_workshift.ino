#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define DOOR_PIN 8

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
    pinMode(DOOR_PIN, OUTPUT);
    digitalWrite(DOOR_PIN, LOW);
}

void loop() {
    // Check for incoming serial commands
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        if (command == "OPEN_DOOR") {
            digitalWrite(DOOR_PIN, HIGH);
            delay(2000); // Keep door open for 2 seconds
            digitalWrite(DOOR_PIN, LOW);
        }
    }

    // Check for RFID card
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        String id = "";
        for (byte i = 0; i < 4; i++) {
            id += String(rfid.uid.uidByte[i], HEX);
        }
        
        // Send ID to Qt application
        Serial.print("ID:");
        Serial.println(id);
        
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
    }
    delay(100);
}