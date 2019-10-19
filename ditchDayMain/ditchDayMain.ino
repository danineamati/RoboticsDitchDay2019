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

// global state variable
int state = 0;
int LEDpin_state1 = 2; // Red
int LEDpin_state2 = 3; // Green
int LEDpin_state3 = 4; // Blue

bool task1_RFID = false;
bool task2_Oscill = false;
bool task3_Joystick = false;

String serialInput = "";

void setup() {
  // initialize LEDs
  pinMode(LEDpin_state1, OUTPUT);
  pinMode(LEDpin_state2, OUTPUT);
  pinMode(LEDpin_state3, OUTPUT);
  
  // initialize Serial (for debugging)
  Serial.begin(9600);
  
  
  // initialize LCD Display
  
  // initialize RFID

  // initialize Ultrasonic Sensor

  // initialize joystick

}

void loop() {
  // Run task loops

  delay(1000);


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

    // move to state 1
    state = 1;
  } 
  else if(state == 1) 
  {
    /*  1) Get me to the cave!
     *        - Moves to State 2 iff task 1 ONLY complete
     *        - Returns to State 0 if any other taks complete
     */

    // LED 1 on
    digitalWrite(LEDpin_state1, HIGH);
    digitalWrite(LEDpin_state2, LOW);
    digitalWrite(LEDpin_state3, LOW);

    // display message

    // if task 2 or 3 is completed, return to state 0
    // else if task 1 is completed, move to state 2
    // otherwise, no change
    if (task2_Oscill || task3_Joystick) {
      state = 0;
    } else if (task1_RFID) {
      state = 2;
    }
  } 
  else if(state == 2) 
  {
    /*  2) Reduce the wall to rubish!
     *        - Moves to State 3 iff task 1&2 complete
     *        - Returns to Stat 0 if task 3 is completed
     */

    // LED 1 and 2 on
    digitalWrite(LEDpin_state1, HIGH);
    digitalWrite(LEDpin_state2, HIGH);
    digitalWrite(LEDpin_state3, LOW);

    // display message

    // if task 3 completed, return to state 0
    // else if task 1 and 2 completed, move to state 3
    // otherwise, no change
    if (task3_Joystick) {
      state = 0;
    } 
    else if(task2_Oscill && task1_RFID) {
      state = 3;
    }
  } 
  else if(state == 3) 
  {
    /*  3) Escape the cave!
     *        - Moves to State 4 iff task 3 complete
     */

    // LED 1, 2 and 3 on
    digitalWrite(LEDpin_state1, HIGH);
    digitalWrite(LEDpin_state2, HIGH);
    digitalWrite(LEDpin_state3, HIGH);

    // display message

    // if taks 3 completed, move to state 4
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

  Serial.print("You are ending at state ** ");
  Serial.print(state);
  Serial.println(" ** ");
  Serial.println();
  Serial.println();

}