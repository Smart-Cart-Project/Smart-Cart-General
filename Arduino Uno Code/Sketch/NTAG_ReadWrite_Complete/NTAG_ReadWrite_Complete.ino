#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

bool isWrite;
bool isRead;

void ntagRead();
void ntagWrite(String productId);


void setup() {

  Serial.begin(115200);  //Initialize Serail communication, make sure it is same for every component
  SPI.begin();           //Initialize SPI Bus
  mfrc522.PCD_Init();    // Initialize MFRC522 module

  isWrite = false;
  isRead = true;
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    String productId = "4dh8owp290cjpl0j";  //Put the ID of the product, which you will write on the NTAG215

    if (isWrite) {
      ntagWrite(productId);
    } else if (isRead) {
      ntagRead();
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

void ntagWrite(String productId) {

  byte data[4];
  // byte pageToWrite = 1;
  // for(byte pageToWrite = 1; pageToWrite < 33; pageToWrite++) {
  for (byte i = 0; i < 4; i++) {
    productId.substring(i * 4, (i * 4) + 4).getBytes(data, sizeof(data) + 1);
    Serial.print("Subpage ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(productId.substring(i * 4, (i * 4) + 4));

    //Replace 1 with "pageToWrite" from the for loop
    MFRC522::StatusCode status = mfrc522.MIFARE_Ultralight_Write(1 * 4 + i, data, sizeof(data));
    if (status == MFRC522::STATUS_OK) {
      Serial.println(" written successfully.");
    } else {
      Serial.println("Error writing data");
    }
  }
  //}
}

void ntagRead() {
  // for (byte page = 1; page < 33; page++) {
  byte buffer[18];
  byte bufferSize = sizeof(buffer);
  memset(buffer, 0, bufferSize);

  //replace 1 with "page" from the for loop
  MFRC522::StatusCode status = mfrc522.MIFARE_Read(1 * 4, buffer, &bufferSize);
  if (status == MFRC522::STATUS_OK) {
    // Serial.print("Page ");
    // Serial.print(page);
    // Serial.print(": ");
    for (byte i = 0; i < 16; i++) {
      if (buffer[i] < 0x10) Serial.print("");
      Serial.write(buffer[i]);
    }
    Serial.print("\n");
  } else {
    Serial.print("Error reading page ");
    // Serial.println(page);
  }
}
