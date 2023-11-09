#include <SPI.h>
#include <RFID.h>
#define SS_PIN 10
#define RST_PIN 9
#include <Servo.h>

Servo servo;
RFID rfid(SS_PIN, RST_PIN);
String rfidCard;
String savedID;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting RFID Reader...");
  SPI.begin();
  rfid.init();
  servo.attach(6);
  servo.write(0);
  savedId = "NaN";
}

void loop() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
      Serial.println(rfidCard);
      if (savedID == "NaN"){
        savedID = rfidCard;

        //TODO: Enter servo code
        servo.write(90);
      }
      else if (rfidCard == savedID){
        savedID = "NaN";

        //TODO: Enter servo code
        servo.write(0);
      }
      else{
        //TODO: Error code for RFID scans
      }
      delay(2000);
    }
    rfid.halt();
  }
}
