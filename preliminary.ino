#include <SPI.h>
#include <RFID.h>
#define SS_PIN 10
#define RST_PIN 9
RFID rfid(SS_PIN, RST_PIN);

String rfidCard;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting RFID Reader...");
  SPI.begin();
  rfid.init();

}

void loop() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
      Serial.println(rfidCard);
      if (rfidCard == "51 214 219 29"){
        Serial.println("Scanned Item: RFID Tag");
      }
      else if (rfidCard == "67 44 134 148"){
        Serial.println("Scanned Item: RFID Card");
      }
      else{
        Serial.println("Scanned Item: Item not in dataset.");
      }
      Serial.println("------------");
      delay(1000);
    }
    rfid.halt();
  }
}
