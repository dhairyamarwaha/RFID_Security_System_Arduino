#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 4 //define green LED pin
#define LED_R 7 //define red LED
#define BUZZER 2 //buzzer pin
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myServo; //define servo name
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myServo.attach(6); //servo pin
  myServo.write(0); //servo start position
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  Serial.println("Put your card to the reader...");
  Serial.println();
  lcd.begin();
  lcd.clear();         
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("PLACE THE CARD");
  lcd.setCursor(0,1);
  lcd.print("ON THE SENSOR");

}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "A6 BE 43 73") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ACESS GRANTED");
    lcd.setCursor(0,1);
    lcd.print("CARD IDENTIFIED");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo.write(180);
    delay(5000);
    myServo.write(0);
    digitalWrite(LED_G, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PLACE THE CARD");
    lcd.setCursor(0,1);
    lcd.print("ON THE SENSOR");
  }

 
 else   {
    Serial.println("ACCESS DENIED");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ACCESS DENIED");
    lcd.setCursor(0,1);
    lcd.print("USER NOT FOUND");
    delay(1000);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PLACE THE CARD");
    lcd.setCursor(0,1);
    lcd.print("ON THE SENSOR");
    
  }
} 
