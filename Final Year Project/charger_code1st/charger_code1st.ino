
//#include <Adafruit_LiquidCrystal.h>
#define RXp2 0
#define TXp2 1

int seconds = 0;
//Adafruit_LiquidCrystal lcd(0);
#include <Wire.h>
#include <CAN.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial Serial1(0, 1);

//0x3F or 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);   //LCD Object


const int PilotPin = 3; //330 ohm optocoupler
const int ChargeRequestPin = 4; //lm393 pin no 7
const int VehicleDetectPin = 5; //lm393 pin no 1
const int PowerRelay = 6;
bool VehicleDetected = false; 
bool ChargeRequested = false;



const int led1 = 9;
const int led2 = 8; 
const int led3 = 7;


//LiquidCrystal lcd(13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3);  /* For 8-bit mode */
//LiquidCrystal lcd(13, 12, 11, 6, 5, 4, 3);  /* For 4-bit mode */

unsigned char Character1[8] = { 0x04, 0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F }; /* Custom Character 1 */
unsigned char Character2[8] = { 0x01, 0x03, 0x07, 0x1F, 0x1F, 0x07, 0x03, 0x01 }; /* Custom Character 2 */


void setup() {
  Serial.begin(9600);
  //Serial1.begin(9600);
  Serial1.begin(9600);

  

  // Setup LCD with backlight and initialize
  lcd.init();
  lcd.backlight();
  pinMode (PilotPin, OUTPUT);
  pinMode (ChargeRequestPin, INPUT_PULLUP);
 /* TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS22);
    OCR2A = 180;
    OCR2B = 50;*/
  pinMode (VehicleDetectPin, INPUT_PULLUP);
  pinMode (PowerRelay, OUTPUT);
  pinMode (led1,OUTPUT);
  pinMode (led2,OUTPUT);
  pinMode (led3,OUTPUT);

  digitalWrite(PowerRelay, LOW);
  analogWrite(PilotPin, 255);
  digitalWrite(led1, OUTPUT);
  
  
  lcd.clear();
  lcd.print("CAN BUS connected");
  lcd.begin(16, 2);
  lcd.createChar(0, Character1);  /* Generate custom character */
  lcd.createChar(1, Character2);
  CAN.begin(500E3);
  
  lcd.setCursor(0, 0);
  delay(1000);
}


void loop() {

   Check();

    if (VehicleDetected == true) {
      digitalWrite(led2, HIGH);
      analogWrite (PilotPin, 64);
      Serial.println("Charger is Connected");
      Serial1.println("Charger is Connected");
      CAN.beginPacket(0x12);
      CAN.write("Charger is Connected");
      lcd.setCursor(0, 0);
  	  lcd.print("Charger is Connected");
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("CAN  msg  sent");
      
  	  lcd.setBacklight(1);
      delay(1000);
  	  
    }

    else{
      digitalWrite(led2, LOW);
      analogWrite(PilotPin, 255);
      Serial.println("Charger is not Connected");
      Serial1.println("Charger is not Connected");
      CAN.beginPacket(0x12);
      CAN.write("Charger is not Connected");
      lcd.setCursor(0, 0);
      lcd.print("Charger is not Connected");
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("CAN  msg  sent");
      lcd.setBacklight(1);
      delay(1000);
  
    }

    if (VehicleDetected == true && ChargeRequested == true ){
      Serial.println("ChargerConnectedandCharging Requested");
      delay(1000); 
      Check();
      if(VehicleDetected == true && ChargeRequested == true){
        digitalWrite(led3, HIGH); 
        digitalWrite(PowerRelay, HIGH);
        Serial.println("PowerRelayisOn");
        Serial1.println("PowerRelayisOn");
        CAN.beginPacket(0x12);
        CAN.write("PowerRelayisOn");
        lcd.setCursor(0, 1);
        lcd.print("PowerRelayisOn");
        delay(1000);
        lcd.setCursor(0, 0);
        lcd.print("CAN  msg  sent");
        lcd.setBacklight(1);
        delay(1000);
      }

      else{
        digitalWrite(led3, LOW); 
        digitalWrite(PowerRelay, LOW);
        Serial.println("PowerRelaisOff"); 
        Serial1.println("PowerRelayisOff");
        CAN.beginPacket(0x12);
        CAN.write("PowerRelayisOff");
        lcd.setCursor(0, 1);
        lcd.print("PowerRelayisOff");
        delay(1000);
        lcd.setCursor(0, 0);
        lcd.print("CAN  msg  sent");
        lcd.setBacklight(1);
        delay(1000);
        
      }
    }
    else{
        digitalWrite(led3, LOW);
        digitalWrite(PowerRelay, LOW);
        Serial.println("PowerRelayisOff");
        Serial1.println("PowerRelayisOff");
        CAN.beginPacket(0x12);
        CAN.write("PowerRelayisOff");
        lcd.setCursor(0, 1);
        lcd.print("PowerRelayisOff");
        delay(1000);
        lcd.setCursor(0, 0);
        lcd.print("CAN  msg  sent");
        lcd.setBacklight(1);
        delay(1000);
      }
}



   
 void Check()  {
  VehicleDetected = !(digitalRead(VehicleDetectPin));
   if(pulseIn(ChargeRequestPin, HIGH, 5000) > 0){
    ChargeRequested = false;
   }
   else{
   ChargeRequested = true;
   } 
}