/* Common functions needed from LCD portion of project

Team: Daniel and Spencer
ME/CS/EE 129

*/

#include "LCD_functions.h"
#include "Arduino.h"

// Global LCD variables
int lcdMaxLength = 16;

/* The simplest print this line1 top and line2 bottom
 */
void dispTextSimplest(LiquidCrystal_PCF8574 myLCD, 
  String line1, String line2) {

  myLCD.setCursor(0,0);
  myLCD.print(line1);
  myLCD.setCursor(0,1);
  myLCD.print(line2);
}


/*
 * Displays two lines of text to the LCD screen. If the text is
 * too long, it will shift left.
 * 
 * INPUT:   myLCD is the LCD object
 *      line1 is the top line of string to display
 *      line2 is the bottom line of string to display
 */ 
void dispTextShift(LiquidCrystal_PCF8574 myLCD, 
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
    do {
      dispTextSimplest(myLCD, line1, line2);
      
      delay(1000); // Delay to allow reading

      myLCD.scrollDisplayLeft();
      counter --;
    } while (counter >= lcdMaxLength);

    delay(1000);
}

/* 
 * Displays an arbitrary length of text as a scroll sequence.
 * No distinction of words is made.
 *
 * Input: myLCD is the LCD object
 *      tot is the whole string to display
 */ 
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


    dispTextSimplest(myLCD, line1, line2);

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

    dispTextSimplest(myLCD, line1, line2);
    delay(3000);
  }

  // We only have one line and can display the remaining text.
  if (totCounter <= lcdMaxLength) {
    line1 = tot.substring(line1Start);

    myLCD.clear();
    myLCD.setCursor(0,0);
    myLCD.print(line1);
  }


}

/* 
 * Displays an arbitrary length of text as a scroll sequence.
 * Distinction IS MADE for parsing words.
 *
 * Input: myLCD is the LCD object
 *      tot is the whole string to display
 */ 
void dispWordsScroll(LiquidCrystal_PCF8574 myLCD,
  String tot) {

  // Clear the LED and set up
  myLCD.setBacklight(100);
    myLCD.home(); myLCD.clear();

    // Parse string
    int numWords = countWords(tot);

    // Edge case, empty sting
    if (numWords <= 0) {
      return;
    }

    String words[numWords];
    parseString(tot, numWords, words);

    // Now the words array is filled up.

    String line1 = " ";
    String line2 = words[0];
    String currentWord = "";

    // Look at each word in the array, determine it's length.
    // Note that the first word has already been added.
    for (int thisWord = 1; thisWord < numWords; thisWord ++) {
      // Check if the line has room for that word
      currentWord = words[thisWord];

      if (currentWord.length() + line2.length() < lcdMaxLength) 
      {
        // if there is room
        // Add the word to the string
        line2 = line2 + " " + currentWord;
        // Move to the next word
      } 
      else 
      {
        // otherwise, the line is full.
        // Print the strings
        myLCD.clear();
        dispTextSimplest(myLCD, line1, line2);
        delay(1000);
        // Move line 2 to line 1
        line1 = line2;
        // Refresh line 2, and add the word to the string
        line2 = currentWord;
      }
    }

    myLCD.clear();
  myLCD.setCursor(0,0);
  myLCD.print(line1);
  myLCD.setCursor(0,1);
  myLCD.print(line2);
  delay(1000);
}

// Determine number of words in a string (based on spaces)
// int countWords(LiquidCrystal_PCF8574 myLCD, String inString) {
int countWords(String inString) {
  int numWords = 0;
  int lastSpace = 0;
  int nextSpace = 0;

  while (nextSpace != -1) {
    nextSpace = inString.indexOf(" ", lastSpace);
    numWords ++;
    lastSpace = nextSpace + 1;

    // dispTextSimple(myLCD, String(numWords), String(nextSpace));
  }

  return numWords;
}

/* 
 * Parse a string into an array of words.
 *
 * INPUT:   inString is the string to parse
 *      numWords is the number of words in the string 
 *        (e.g. from using the count words function)
 *      words is the output word array
 *
 * OUTPUT:  the word array (modified from what was passed in)
 */
void parseString(String inString, int numWords, String* words) {

  int counter = 0;
  int lastSpace = 0;
  int nextSpace = 0;

  while (counter < numWords) {
    nextSpace = inString.indexOf(" ", lastSpace);

    if (nextSpace == -1) {
      // If there are no spaces left, take the rest of the
      // string.
      words[counter] = inString.substring(lastSpace);
      break;
    } else {
      words[counter] = inString.substring(lastSpace, nextSpace);
    }

    lastSpace = nextSpace + 1;
    counter ++;
    
  }
}
