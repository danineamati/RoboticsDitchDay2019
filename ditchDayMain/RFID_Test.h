#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#include "LCD_functions.h"
 
#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

// Change to the UID of the card that you want to give access
String accessUID = "79 0B 38 8E";

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void RFIDsetup();
void RFIDsetup(LiquidCrystal_PCF8574 myLCD);
void RFIDloop();
void RFIDloop(LiquidCrystal_PCF8574 myLCD);