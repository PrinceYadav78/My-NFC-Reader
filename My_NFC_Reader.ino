#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>

#define SDA_PIN 8
#define SCL_PIN 9
#define PN532_RST 10
#define PN532_IRQ 7

#define BUZZER_PIN 12
#define RELAY_PIN 11

PN532_I2C pn532_i2c(Wire);
PN532 nfc(pn532_i2c);

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println("Starting NFC Reader...");

  nfc.begin();

  uint32_t version = nfc.getFirmwareVersion();
  if (!version) {
    Serial.println("PN532 NOT FOUND");
    while (1);
  }

  Serial.print("PN532 Firmware: ");
  Serial.println((version >> 16) & 0xFF, HEX);

  nfc.SAMConfig();
  Serial.println("Waiting for NFC card...");
}

void loop() {
  uint8_t uid[7];
  uint8_t uidLength;

  bool success = nfc.readPassiveTargetID(
    PN532_MIFARE_ISO14443A,
    uid,
    &uidLength
  );

  if (success) {
    Serial.print("Card UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // Beep
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);

    // Relay ON
    digitalWrite(RELAY_PIN, HIGH);
    delay(1000);
    digitalWrite(RELAY_PIN, LOW);

    delay(1500);
  }
}
