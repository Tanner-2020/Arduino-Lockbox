#include <SPI.h>
#include <RFID.h>
#include <Servo.h>
#define SS_PIN 10
#define RST_PIN 9

Servo servo;
RFID rfid(SS_PIN, RST_PIN);
String currentRfid;
String savedRfid;

void setup(){
  Serial.begin(9600); // 9600 bits per second
  SPI.begin(); 
  rfid.init();
  servo.attach(6); // servo motor's port/   servo.write(0); // initialize to unlocked position
  savedRfid[0] = -1; // default RFID value
  pinMode(5, OUTPUT); //red LED
  pinMode(3, OUTPUT); //green LED
  servo.write(90);
  savedRfid = String(-1);
  currentRfid = String(-1);

  // Receives status and id values from Pi via serial exchange
  int status = -1;
  while (status == -1) {
    Serial.println("Status");
    status = Serial.readString().toInt();
  }
  int id = -1;
  while (id == -1) {
    Serial.println("ID");
    id = Serial.readString().toInt();
  }

  // Initializes lock settings to match last saved status
  if (status == 1) {
    servo.write(90);
    digitalWrite(5, HIGH);
    digitalWrite(3, LOW);
    savedRfid = String(id)
  } else {
    servo.write(0);
    digitalWrite(5, LOW);
    digitalWrite(3, HIGH);
  }

}

void loop() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
	    currentRfid = String(rfid.serNum[0]) + "" + String(rfid.serNum[1]) + "" + String(rfid.serNum[2]) + "" + String(rfid.serNum[3]);

      if (savedRfid == "-1"){ // to lock
        savedRfid = currentRfid;
        digitalWrite(5, HIGH);
        digitalWrite(3, LOW);
        Serial.println("1 1 " + String(rfid.serNum[0]) + "" + String(rfid.serNum[1]) + "" + String(rfid.serNum[2]) + "" + String(rfid.serNum[3]));
        servo.write(90);
      } else if (currentRfid == savedRfid) { // to unlock
        savedRfid = "-1";
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
        digitalWrite(5, LOW);
        digitalWrite(3, HIGH);
        Serial.println("0 1 " + String(rfid.serNum[0]) + "" + String(rfid.serNum[1]) + "" + String(rfid.serNum[2]) + "" + String(rfid.serNum[3]));
      }
      delay(2000);
    }
    rfid.halt();
  }
}