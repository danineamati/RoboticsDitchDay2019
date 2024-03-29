/*
 * By Daniel Neamati and Spencer Morgenfeld
 * 
 * ME/CS/EE 129: Experimentatl Robotics
 * 
 * 
 * 
 * Tasks:
 *  1) Bring the arduinos to the RFID sensor (hidden)
 *  2) Oscillate object in front of Ultrasonic sensor at specific freq
 *  3) Move joystrick to avoid the walls
 * 
 * States:
 *  0) Intro
 *        - Moves to State 1
 *  1) Get me to the cave!
 *        - Moves to State 2 iff task 1 ONLY complete
 *        - Returns to State 0 if any other taks complete
 *  2) Reduce the wall to rubish!
 *        - Moves to State 3 iff task 1&2 complete
 *        - Returns to Stat 0 if task 3 is completed
 *  3) Escape the cave!
 *        - Moves to State 4 iff task 3 complete
 *  4) Return Home!
 *        - End State
 */

// import subfunctions
#include "LCD_functions.h"
#include "RFID_functions.h"

#include <MFRC522.h>
#include <Wire.h>
#include <Servo.h>

// LCD Set up
LiquidCrystal_PCF8574 lcd(0x27);  
// set the LCD address to 0x27 for a 16 chars and 2 line display

// RFID Set up
#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above
String accessUID = "79 0B 38 8E";
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// global state variable
int state = 0;
int LEDpin_state1 = 2; // Green
int LEDpin_state2 = 3; // Green
int LEDpin_state3 = 4; // Green
int LEDpin_state4 = 7; // red failure

bool task1_RFID = false;
bool task2_Oscill = false;
bool task3_Joystick = false;

bool debug = false;

unsigned long lastMillis;

String serialInput = "";

Servo myservo;

void setup() {
  // initialize LEDs
  pinMode(LEDpin_state1, OUTPUT);
  pinMode(LEDpin_state2, OUTPUT);
  pinMode(LEDpin_state3, OUTPUT);
  pinMode(LEDpin_state4, OUTPUT);

  myservo.attach(9);
  myservo.write(0);
  
  // initialize Serial (for debugging)
  Serial.begin(9600);
  
  // initialize LCD Display
  Wire.begin();
  Wire.beginTransmission(0x27); //Your LCD Address
  int error = Wire.endTransmission();
  Serial.print("Start Up Status: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
  } else {
    Serial.println(": LCD not found.");
  }

  lcd.begin(16, 2); // initialize the lcd (16 char, 2 lines)
  lcd.setBacklight(100);
  //dispTextSimplest(lcd, "Welcome!", "LCD: Ready");
  delay(1000);
  
  // initialize RFID
  RFIDsetup(lcd);

  // initialize Ultrasonic Sensor
  init_Sonic();
  // initialize joystick
  init_joystick();
}

void loop() {
  lastMillis = millis();
  if (state != 0) {
    // Run task loops

    // Task 1 - Bring the arduinos to the RFID sensor (hidden)
    //RFIDloop(lcd);
    
    // Task 2 - Reduce the wall to rubish!

    // Task 3 - Escape the cave!
    
    
  }
  

  if (debug) {
    // Display state for debugging
    Serial.print("You are currently at state ** ");
    Serial.print(state);
    Serial.println(" ** ");
  
    // Display task status for debugging
    
    Serial.println("Task Status:");
    Serial.print("  1) Task 1 (RFID) =     ");
    Serial.println(task1_RFID);
    Serial.print("  2) Task 2 (Oscil) =    ");
    Serial.println(task2_Oscill);
    Serial.print("  3) Task 3 (JoyStck) =  ");
    Serial.println(task3_Joystick);
    Serial.println(); 
  }

  // Master override

  if (Serial.available() > 0) {    // is a character available?
    int serialNum = Serial.read();       // get the character
  
    // check if a number was received
//    serialInput.toInt();
    if ((serialNum >= '0') && (serialNum <= '9')) 
    {
      Serial.print("Number received: ");
      Serial.println(serialNum - 48);

      int taskNumOverride = serialNum - 48;

      if (taskNumOverride == 1) {
        task1_RFID = true;
      } else if(taskNumOverride == 2) {
        task2_Oscill = true;
      } else if(taskNumOverride == 3) {
        task3_Joystick = true;
      }
    } else if(serialNum == 10) {
      Serial.println("End of Line");
    } else 
    {
      Serial.println("Not a number.");
      Serial.println(serialNum);
    }
  }

  // Check state
  if (state == 0) {
    // intro state

    // reset all task statuses
    task1_RFID = false;
    task2_Oscill = false;
    task3_Joystick = false;
    
    // No LEDS
    digitalWrite(LEDpin_state1, LOW);
    digitalWrite(LEDpin_state2, LOW);
    digitalWrite(LEDpin_state3, LOW);
    
    // display intro message
    dispWordsScroll(lcd, "Only the diamond in the rough may enter the Cave of Wonders!!!");
    dispWordsScroll(lcd, "Find the magic lamp, but be careful of the obstacles ahead");

    // move to state 1
    state = 1;
  } 
  else if(state == 1) 
  {
    /*  1) Get me to the cave!
     *        - Moves to State 2 iff task 1 ONLY complete
     *        - Returns to State 0 if any other taks complete
     */

    task2_Oscill = task2_Oscill || runOsc();
    // task3_Joystick = task3_Joystick || getJoystick(false);
    int joy = getJoystick(false);
    if (joy == 2) {
      task3_Joystick = true;
    }
    
    // LED 1 on
    digitalWrite(LEDpin_state1, LOW);
    digitalWrite(LEDpin_state2, LOW);
    digitalWrite(LEDpin_state3, LOW);

    // display message
    dispWordsScroll(lcd, "Find the entrance!");
    RFIDloop(lcd);
    // if task 2 or 3 is completed, return to state 0
    // else if task 1 is completed, move to state 2
    // otherwise, no change
    if (task2_Oscill || task3_Joystick) {
      state = 0;

      dispWordsScroll(lcd, "Oh oh no! I openned the wrong door!");
      dispWordsScroll(lcd, "Aaaaaahhhh! I got catapulted out of the cave.");
      dispWordsScroll(lcd, "Hmmm. Where am I?");
      digitalWrite(LEDpin_state4, LOW);
      delay(250);
      digitalWrite(LEDpin_state4, HIGH);
      delay(250);
      digitalWrite(LEDpin_state4, LOW);
      delay(250);
      digitalWrite(LEDpin_state4, HIGH);
      delay(250);
      digitalWrite(LEDpin_state4, LOW);
      delay(250);

    } else if (task1_RFID) {
      state = 2;

      // display message
      dispWordsScroll(lcd, "Is that sound coming from the wall?");
      dispWordsScroll(lcd, "Maybe if you wave your hand at the resonant frequency and correct distance, the wall will crumble...");
    }
  } 
  else if(state == 2) 
  {
    /*  2) Reduce the wall to rubish!
     *        - Moves to State 3 iff task 1&2 complete
     *        - Returns to Stat 0 if task 3 is completed
     */
    //task2_Oscill = true;
    task2_Oscill = task2_Oscill || runOsc();
    int joy = getJoystick(false);
    if (joy == 2) {
      task3_Joystick = true;
    }
    
    // LED 1 and 2 on
    digitalWrite(LEDpin_state1, HIGH);
    digitalWrite(LEDpin_state2, LOW);
    digitalWrite(LEDpin_state3, LOW);

    

    // if task 3 completed, return to state 0
    // else if task 1 and 2 completed, move to state 3
    // otherwise, no change
    if (task3_Joystick) {
      state = 0;

      dispWordsScroll(lcd, "Oh oh no! I touched an ancient rune!");
      dispWordsScroll(lcd, "Aaaaaahhhh! I got catapulted out of the cave.");
      dispWordsScroll(lcd, "Hmmm. Where am I?");
      digitalWrite(LEDpin_state4, LOW);
      delay(250);
      digitalWrite(LEDpin_state4, HIGH);
      delay(250);
      digitalWrite(LEDpin_state4, LOW);
      delay(250);
      digitalWrite(LEDpin_state4, HIGH);
      delay(250);
      digitalWrite(LEDpin_state4, LOW);
      delay(250);
    } 
    else if(task2_Oscill && task1_RFID) {
      state = 3;
      

      // display message
      dispWordsScroll(lcd, "A golden door appeared behind the wall.");
      dispWordsScroll(lcd, "The ceiling is collapsing! I need to move through the maze of falling rocks! Hurry!");
      resetJoystick();
    }
  } 
  else if(state == 3) 
  {
    /*  3) Escape the cave!
     *        - Moves to State 4 iff task 3 complete
     */

    //task2_Oscill = task2_Oscill || runOsc();
    task3_Joystick = task3_Joystick || getJoystick(true);
    
    // LED 1, 2 and 3 on
    digitalWrite(LEDpin_state1, HIGH);
    digitalWrite(LEDpin_state2, HIGH);
    digitalWrite(LEDpin_state3, LOW);



    // if state 3 completed, move to state 4
    // otherwise, no change
    if (task3_Joystick) {
      state = 4;
    }

  } 
  else if(state == 4) 
  {
    /*  4) Return Home!
     *        - End State
     */

    // LED 1, 2 and 3 blinking
    digitalWrite(LEDpin_state1, LOW);
    digitalWrite(LEDpin_state2, LOW);
    digitalWrite(LEDpin_state3, LOW);

    delay(500);

    digitalWrite(LEDpin_state1, HIGH);
    digitalWrite(LEDpin_state2, HIGH);
    digitalWrite(LEDpin_state3, HIGH);

    // display message
    myservo.write(100);
    dispWordsScroll(lcd, "CONGRATULATIONS!! YOU RECOVERED THE MAGIC LAMP!");

  } 
  else 
  {
    /* Something has gone wrong
     * display an error, return all variables to false, and return to state 0
     */

    // display error

    task1_RFID = false;
    task2_Oscill = false;
    task3_Joystick = false;

    state = 0;

  }
  if (debug) {
    Serial.print("You are ending at state ** ");
    Serial.print(state);
    Serial.println(" ** ");
    Serial.println();
    Serial.println();
  }
  delay(min(100 - (millis() - lastMillis),100));

}
