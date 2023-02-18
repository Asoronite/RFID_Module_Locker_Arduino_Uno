
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define RST_PIN         9          
#define SS_PIN          10      

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

Servo myservo;
int pos = "";
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
byte accessUID[4]={0x93, 0x41, 0x1A, 0x13};
void setup() {
  myservo.attach(8);

  // lcd.begin(16, 2);
  // lcd.print("Du Hast Geschaft");

	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {

	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}






  Serial.print("UID tag :");
  String cardID= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     cardID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     cardID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }













	// Dump debug info about the card; PICC_HaltA() is automatically called
if(mfrc522.uid.uidByte[0 ]== accessUID[0] && mfrc522.uid.uidByte[1] == accessUID[1] && mfrc522.uid.uidByte[2] == accessUID[2] && mfrc522.uid.uidByte[3] == accessUID[3]){
    Serial.println("ACCESS GRANTED");
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos); 

            
  }
  delay(1000);
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              
  }
    lcd.begin(16, 2);
    lcd.print("ACCESS GRANTED");
    lcd.setCursor(0, 1);
    lcd.print("ID:"+cardID);
  }
  else{
    Serial.println("ACCESS DENIED");
    lcd.begin(16, 2);
    lcd.print("ACCESS DENIED"); 
    lcd.setCursor(0, 1);
    lcd.print("ID:"+cardID);
  }
  mfrc522.PICC_HaltA();
	
}
