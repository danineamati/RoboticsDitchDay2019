#include "LedControl.h"
short wallC = 10;
short wallCMax = 14;
short wallMC = 10;
short wallMCMax = 13;
unsigned long startMillis = 0;
bool newWall[8];
bool b = false;
long frameboi = 0;

int VRy = A0;
int VRx = A1;
int xPos = 0;
int yPos = 0;
short pos[3] = {0, 0};
short posx, posy;
bool walls[9][9];

LedControl lc = LedControl(11,13,12,1); // Pins: DIN,CLK,CS, # of displays connected

byte X[8] =
{
  B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001
};

byte check[8] =
{
  B00010000,
  B00100000,
  B01000000,
  B00100000,
  B00010000,
  B00001000,
  B00000100,
  B00000010
};

void init_joystick() {
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  resetMatrix();
}

bool getJoystick(bool onThis) {
  frameboi++;
  //Serial.println(analogRead(VRx));
  xPos = map(analogRead(VRx), 0, 700, -512, 512);
  yPos = map(analogRead(VRy), 0, 700, -512, 512);

  
  if (abs(xPos) < 200) {xPos = 0;}
  if (abs(yPos) < 200) {yPos = 0;}
  //Serial.println(xPos);
  xPos = scaleInput(xPos);
  yPos = scaleInput(yPos);
  
  pos[0] = max(0, min(7, xPos + pos[0]));
  pos[1] = max(0, min(7, yPos + pos[1]));
  
  wallMC--;
  if (wallMC <= 10 || getDelta() > 5000 && wallMC <= 11) {
    wallMC = wallMCMax;

    for (int i = 0; i < 8; i++) {
      newWall[i] = false;
    }
    wallC--;

    if (wallC <= 10) {
      generateWall(newWall);
      wallC = wallCMax;
    }
  
    // move walls
    for (int i = 7; i >= 0; i--) {
      for (int j = 0; j < 8; j++) {
        if (i == 0) {
          walls[i][j] = newWall[j];
          //Serial.print(newWall[j]);
        }
        else {
          walls[i][j] = walls[i-1][j];
        }
      }
    }
    
  }
  
  drawScreen();
  //Serial.print(pos[0]);
  //Serial.print(" ");
  //Serial.println(pos[1]);
  if (onThis && walls[7-pos[0]][7-pos[1]]) {
    for (int i = 0; i < 5; i++) {
      lc.setLed(0, pos[1], pos[0], b);
      b = !b;
      delay(75);
    }
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < 8; i++)  
      {
        lc.setRow(0,i,X[i]);
      }
      delay(500);
      lc.clearDisplay(0);
      delay(500);
    }
    reset();
  }
  if (onThis && getDelta() > 10000) {
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < 8; i++)  
      {
        lc.setRow(0,i,check[i]);
      }
      delay(500);
      lc.clearDisplay(0);
      delay(500);
    }
    reset();
    
    return true;
  }
  //Serial.println(getDelta());
  return false;
  
}

void drawScreen() {
  lc.clearDisplay(0);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (walls[i][j]) {
        lc.setLed(0, 7-j, 7-i, true);
      }
    }
  }
  lc.setLed(0, pos[1], pos[0], b);
  b = !b;
}

void generateWall(bool wall[8]) {
  int lower = 2;
  if (getDelta() > 2500) {lower = 3;}
  if (getDelta() > 7500) {lower = 4;}
  int numWalls = random(lower, 6);
  int center = random(0,7);
  for (int i = 0; i < 8; i++) {
    wall[i] = false;
  }
  for (int i = 0; i < numWalls; i++) {
    wall[center] = true;
    center++;
  }
}

int scaleInput(int toScale) {
  if (abs(toScale) > 50) {return -1*sign(toScale);}
  return 0;
}

int sign(int x) {
    return (x > 0) - (x < 0);
}

void reset() {
  lc.clearDisplay(0);
  for(int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++) {
      walls[i][j] = false;
    }
  }
  resetMatrix();
  startMillis = millis();
  
}

long getDelta() {
  return (millis() - startMillis);
}

void resetMatrix() {
  pos[0] = 0;
  pos[1] = 0;
  lc.shutdown(0,false);  // Wake up display
  lc.setIntensity(0,5);  // Set intensity level
  lc.clearDisplay(0);  // Clear Display
}
