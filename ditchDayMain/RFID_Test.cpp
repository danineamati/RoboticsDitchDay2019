/*
 * By Daniel Neamati and Spencer Morgenfeld
 * 
 * Original code inspired by FILIPEFLOP'
 * Inspired by project on: https://randomnerdtutorials.com/
 * 
 */
 
#include "RFID_Test.h"

void RFIDsetup()
{
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Find your card and bring it to the reader...");
  Serial.println();
}

void RFIDsetup(LiquidCrystal_PCF8574 myLCD)
{
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  dispWordsScroll(myLCD, "RFID: Find your card and bring it to the reader...");
}

void RFIDloop() 
{
  // Reset the loop if no new card present on the sensor/reader. 
  // This saves the entire process when idle.
//  Serial.println("Start of Loop");
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

  // Dump debug info about the card; PICC_HaltA() is automatically called
//  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String thisUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     thisUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     thisUID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  thisUID.toUpperCase();

  // Substring starts at 1 because there is an extra space
  if (thisUID.substring(1) == accessUID)
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(3000);
  }
 
 else   {
    Serial.println(" Access denied");
    delay(3000);
  }
} 

void RFIDloop(LiquidCrystal_PCF8574 myLCD) 
{
  // Reset the loop if no new card present on the sensor/reader. 
  // This saves the entire process when idle.
//  Serial.println("Start of Loop");
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

  // Dump debug info about the card; PICC_HaltA() is automatically called
//  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  
  //Show UID on serial monitor
  dispWordsScroll(myLCD, "UID tag :");
  String thisUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     thisUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     thisUID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  thisUID.toUpperCase();
  dispTextShift(myLCD, thisUID, "");

  // Substring starts at 1 because there is an extra space
  if (thisUID.substring(1) == accessUID)
  {
    dispWordsScroll(myLCD, "Authorized access");
    delay(3000);
  }
 
 else   {
    dispWordsScroll(myLCD, "Access denied");
    delay(3000);
  }
} 
