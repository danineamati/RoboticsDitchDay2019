/* Common functions needed from LCD portion of project

Team: Daniel and Spencer
ME/CS/EE 129

*/

#include "LCD_functions.h"
#include "Arduino.h"

// Global LCD variables
int lcdMaxLength = 16;

void dispTextSimple(LiquidCrystal_PCF8574 myLCD, 
	String line1, String line2) {

	// Clear the LED and set up
	myLCD.setBacklight(100);
    myLCD.home(); myLCD.clear();
    myLCD.setCursor(0,0);

    // Use the string length to determine if scrolling is needed
    // Note that these will act as counters.
    int line1Len = line1.length();
    int line2Len = line2.length();

    int counter = max(line1Len, line2Len);


    // While the line characters are such that the message has not
    // been fully displayed, then the shift left
    while (counter >= lcdMaxLength) {
    	myLCD.setCursor(0,0);
    	myLCD.print(line1);
	    myLCD.setCursor(0,1);
	    myLCD.print(line2);
	    
	    delay(1000); // Delay to allow reading

	    myLCD.scrollDisplayLeft();
	    counter --;
    }
    delay(1000);
}


void dispTextScroll(LiquidCrystal_PCF8574 myLCD,
	String tot) {

	// Clear the LED and set up
	myLCD.setBacklight(100);
    myLCD.home(); myLCD.clear();

    // Store the string length as a counter
    int totCounter = tot.length();

    // Initialize top line, which will be subsets of the original string
    String line1 = "";
    String line2 = "";

    // determine indicies.
    int line1Start = 0;
    int line2Start = min(lcdMaxLength, totCounter);
    // int line2End = min(line2Start + lcdMaxLength, totCounter);

    myLCD.clear();
    myLCD.setCursor(0,0);
    myLCD.print(String(totCounter));
    delay(1000);

    while (totCounter > 2 * lcdMaxLength) {
    	myLCD.clear();

    	line1 = tot.substring(line1Start, line1Start + lcdMaxLength);
    	line2 = tot.substring(line2Start, line2Start + lcdMaxLength);


    	myLCD.setCursor(0,0);
    	myLCD.print(line1);
    	myLCD.setCursor(0,1);
    	myLCD.print(line2);

    	totCounter -= lcdMaxLength; // Moved one whole line
    	line1Start += lcdMaxLength; // Move line 2 to line 1
    	line2Start += lcdMaxLength; // Move line 3 to line 2

    	delay(3000);
    }

    // We only have two lines and can display the remaining text.
    if (totCounter > lcdMaxLength) {
    	myLCD.clear();
    	myLCD.setCursor(0,0);
    	myLCD.print("Only 2 lines");

    	line1 = tot.substring(line1Start, line1Start + lcdMaxLength);
    	line2 = tot.substring(line2Start);

    	myLCD.setCursor(0,0);
    	myLCD.print(line1);
    	myLCD.setCursor(0,1);
    	myLCD.print(line2);

    	delay(3000);
    }

    // // We only have one line and can display the remaining text.
    // if (totCounter <= lcdMaxLength) {
    // 	line1 = tot.substring(line1Start);

    // 	myLCD.clear();
    // 	myLCD.setCursor(0,0);
    // 	myLCD.print(line1);
    // }


}