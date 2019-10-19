/*
 * created by Rui Santos, https://randomnerdtutorials.com
 * 
 * Complete Guide for Ultrasonic Sensor HC-SR04
 *
    Ultrasonic sensor Pins:
        VCC: +5VDC
        Trig : Trigger (INPUT) - Pin11
        Echo: Echo (OUTPUT) - Pin 12
        GND: GND
 */

// Ultrasonic Sensor
int trigPin = 41;    // Trigger
int echoPin = 40;    // Echo
float cm, lastCM;
int period = 1000;
int error = 500;
int eqlDist = 10;
int initOscCt = 5;
int curOscCt = initOscCt;
unsigned long lastMillis, lastPMillis;
int curDelta;
bool passedOsc;
 
void setup() {
  // Serial Port begin
  Serial.begin (9600);
  // Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  passedOsc = false;
  resetOsc();
}

void resetOsc() {
  // Reset all Osc variables 
  curOscCt = initOscCt;
  lastCM = 0;
  lastMillis = 0;
  curDelta = 0;
  lastPMillis = 0;
}

bool runOsc() {
  // Call from main loop; returns whether it has passed the ultrasonic challenge
  lastMillis = millis();
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
 
  // Convert the time into a distance
  cm = (pulseIn(echoPin, HIGH)/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343

  curDelta = millis() - lastPMillis;
  
  if (cm > eqlDist && lastCM < eqlDist || cm < eqlDist && lastCM > eqlDist) {
    lastPMillis = millis();
    if (curDelta > period - error && curDelta < period + error) {
      curOscCt -= 1;
    }
    else {
      curOscCt = initOscCt;
    }
  }
  
  Serial.print(cm);
  Serial.print("cm ");
  Serial.print(curOscCt);
  Serial.println();
  lastCM = cm;

  //delay to exactly 100 ms
  
  delay(min(100 - (millis() - lastMillis),100));
  return curOscCt <= 0;
}
 
void loop() {
  passedOsc = runOsc();
}
