/* Common functions needed from LCD portion of project

Team: Daniel and Spencer
ME/CS/EE 129

*/

#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>

void dispTextSimple(LiquidCrystal_PCF8574 myLCD, 
	String line1, String line2);

void dispTextScroll(LiquidCrystal_PCF8574 myLCD,
	String tot);