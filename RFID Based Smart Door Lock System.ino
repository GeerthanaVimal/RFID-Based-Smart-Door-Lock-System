#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h> 

#define SS_PIN 10
#define RST_PIN 9

LiquidCrystal_I2C lcd(0x3F,16,2);
MFRC522 mfrc522(SS_PIN, RST_PIN);  
Servo servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Started");
  
  servo.attach(3);
  servo.write(0);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,3);
  lcd.print("Hello");

}

void idle_display()
{
  lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("To enter show");
    lcd.setCursor(0,1);
    lcd.print(" your tag :-)");
}
void loop() {

// put your main code here, to run repeatedly:
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  String content= "";
  byte letter;
  String card;


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

  unsigned int temp=0;
  int a=0;

  if (content.substring(1) == "49 CF AC 9E") //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Authorized access");
    Serial.println();
    servo.write(90);
    //////////////////////////Wait for 10 seconds and then lock the door automatically.
    temp=millis();   
    while(millis()-temp<=10000)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Authorized");
      lcd.setCursor(0,1);
      lcd.print("  Access   ");
      lcd.print(10-a);
      lcd.print("Sec");
      a++;
      delay(1000);
    }
    servo.write(0);
    idle_display();
  }
 
 else   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Access denied");
    delay(3000);
  }
  idle_display();
}
