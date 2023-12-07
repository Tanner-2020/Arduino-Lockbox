#include <SPI.h>
#include <RFID.h>
#include <Servo.h>
#define SS_PIN 10
#define RST_PIN 9

Servo servo;
RFID rfid(SS_PIN, RST_PIN);
String currentRfid;
String savedRfid;
int lock = 90;
int unlock = 0;

void setup(){
  Serial.begin(9600); // 9600 bits per second
  SPI.begin(); 
  rfid.init();
  servo.attach(6); // servo motor's port/   servo.write(0); // initialize to unlocked position
  savedRfid[0] = -1; // default RFID value
  pinMode(5, OUTPUT); //red LED
  pinMode(3, OUTPUT); //green LED
  servo.write(lock);
  savedRfid = String(-1);
  currentRfid = String(-1);

  // Receives status and id values from Pi via serial exchange
  int status = -1;
  while (status != 1 && status != 2) {
    servo.write(lock);
    Serial.println("Status");
    status = Serial.readString().toInt();
    //if (status == NULL) {
	//status = -1;
	//}
  }
  Serial.println(String(status));
  long id = -1;
  while (id < 0) {
    Serial.println("ID");
    id = atol(Serial.readString().c_str());
    //if (id == NULL) {
	//id = -1;
	//}
  }

  // Initializes lock settings to match last saved status
  if (status == 1) {
    servo.write(lock);
    digitalWrite(5, HIGH);
    digitalWrite(3, LOW);
    savedRfid = String(id);
  } else {
    servo.write(unlock);
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
        servo.write(lock);
      } else if (currentRfid == savedRfid) { // to unlock
        savedRfid = "-1";
        Serial.println("1 2 " + String(rfid.serNum[0]) + "" + String(rfid.serNum[1]) + "" + String(rfid.serNum[2]) + "" + String(rfid.serNum[3]));
        digitalWrite(5, LOW);
        digitalWrite(3, HIGH);
        servo.write(unlock);
      } else { // to remain locked if RFID does not work
	Serial.println(currentRfid);
	Serial.println(savedRfid);
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
