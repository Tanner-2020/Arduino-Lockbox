#include <SPI.h>
#include <RFID.h>
#include <Servo.h>
#define SS_PIN 10
#define RST_PIN 9

Servo servo;
RFID rfid(SS_PIN, RST_PIN);
int currentRfid[4];
int savedRfid[4];

void setup() {
  Serial.begin(9600); // 9600 bits per second
  //Serial.println(1);
  SPI.begin(); 
  rfid.init();
  servo.attach(6); // servo motor's port
  servo.write(0); // initialize to unlocked position
  savedRfid[0] = -1; // default RFID value
  pinMode(5, OUTPUT); //red LED
  pinMode(3, OUTPUT); //green LED
}

void loop() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
	    currentRfid[0] = rfid.serNum[0];
	    currentRfid[1] = rfid.serNum[1];
	    currentRfid[2] = rfid.serNum[2];
	    currentRfid[3] = rfid.serNum[3];

      if (savedRfid[0] == -1){ // to lock
        savedRfid[0] = currentRfid[0];
	      savedRfid[1] = currentRfid[1];
	      savedRfid[2] = currentRfid[2];
	      savedRfid[3] = currentRfid[3];
        digitalWrite(5, HIGH);
        digitalWrite(3, LOW);
        Serial.println("1 1 " + String(rfid.serNum[0]) + "" + String(rfid.serNum[1]) + "" + String(rfid.serNum[2]) + "" + String(rfid.serNum[3]));
        servo.write(90);
      } else if (currentRfid[0] == savedRfid[0] && currentRfid[1] == savedRfid[1] && currentRfid[2] == savedRfid[2] && currentRfid[3] == savedRfid[3]) { // to unlock
        savedRfid[0] = -1;
        Serial.println("1 0 " + String(rfid.serNum[0]) + "" + String(rfid.serNum[1]) + "" + String(rfid.serNum[2]) + "" + String(rfid.serNum[3]));
        digitalWrite(5, LOW);
        digitalWrite(3, HIGH);
        servo.write(0);
      } else { // to remain locked if RFID does not work
        for(int i = 0; i < 10; i++){
          digitalWrite(5, i % 2 == 0 ? HIGH : LOW);
          digitalWrite(3, i % 2 == 0 ? LOW : HIGH);
          delay(250);
        }
        Serial.println("0 1 " + String(rfid.serNum[0]) + "" + String(rfid.serNum[1]) + "" + String(rfid.serNum[2]) + "" + String(rfid.serNum[3]));
      }
      delay(2000);
    }
    rfid.halt();
  }
}
