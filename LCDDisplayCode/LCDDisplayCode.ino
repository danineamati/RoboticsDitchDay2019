#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include "LCD_functions.h"

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int show;

void setup()
{
  int error;

  Serial.begin(115200);
  Serial.println("LCD...");

  // See http://playground.arduino.cc/Main/I2cScanner
  Wire.begin();
  Wire.beginTransmission(0x27); //Your LCD Address
  error = Wire.endTransmission();
  Serial.print("Start Up Status: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
  } else {
    Serial.println(": LCD not found.");
  }

  lcd.begin(16, 2); // initialize the lcd
  show = 0;
}

void loop()
{

  // Test left shift text
  if (true) {
    dispTextSimple(lcd, "Hello World!", "From your man :)");
    delay(1000);
  
    lcd.setBacklight(0);
    delay(400);
    lcd.setBacklight(255);
  }

  // Test Scrolling Text without parsings
  if (false) {
    dispTextScroll(lcd, "Hello World from an LCD. From your main man :). Welcome to Arduino Land");
    delay(1000);

    lcd.setBacklight(0);
    delay(400);
    lcd.setBacklight(255);
  }

  // Test String Comprehension
  if (false) {
    int numWords = countWords("1 2 3 4 5 6");
    dispTextSimple(lcd, String(numWords), "");
  
    String words[numWords];
    parseString("1 2 3 4 5 6", numWords, words);
  
    for (int i = 0; i < numWords; ++ i) {
      Serial.println(words[i]);
    }
  }
  
  // Test Scrolling Text WITH parsing
  if (true) {
    dispWordsScroll(lcd, "Hello World from an LCD. From your main man :). Welcome to Arduino Land");
    delay(1000);

    lcd.setBacklight(0);
    delay(400);
    lcd.setBacklight(255);
  }

//  } 
//    else if (show == 1) {
//    lcd.clear();
//    lcd.print("Cursor On");
//    lcd.cursor();
//
//  } else if (show == 2) {
//    lcd.clear();
//    lcd.print("Cursor Blink");
//    lcd.blink();
//
//  } else if (show == 3) {
//    lcd.clear();
//    lcd.print("Cursor OFF");
//    lcd.noBlink();
//    lcd.noCursor();
//
//  } else if (show == 4) {
//    lcd.clear();
//    lcd.print("Display Off");
//    lcd.noDisplay();
//
//  } else if (show == 5) {
//    lcd.clear();
//    lcd.print("Display On");
//    lcd.display();
//
//  } else if (show == 7) {
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("*** first line.");
//    lcd.setCursor(0, 1);
//    lcd.print("*** second line.");
//
//  } else if (show == 8) {
//    lcd.scrollDisplayLeft();
//  } else if (show == 9) {
//    lcd.scrollDisplayLeft();
//  } else if (show == 10) {
//    lcd.scrollDisplayLeft();
//  } else if (show == 11) {
//    lcd.scrollDisplayRight();
//  }

  delay(2000);
  // show = (show + 1) % 12;
}
