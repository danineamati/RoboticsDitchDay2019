/* Common functions needed from RFID portion of project

Team: Daniel and Spencer
ME/CS/EE 129

*/

#ifndef RFID_FUNCTIONS
#define RFID_FUNCTIONS 

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#include "LCD_functions.h"

// Global RFID variables
// Change to the UID of the card that you want to give access
extern String accessUID;
extern MFRC522 mfrc522;   // Create MFRC522 instance.

void RFIDsetup();
void RFIDsetup(LiquidCrystal_PCF8574 myLCD);
void RFIDloop();
void RFIDloop(LiquidCrystal_PCF8574 myLCD);

#endif
