#include "joystick.h"
#include "util.h"

/****************** Const Variables Declarations ******************/

const int rowNum = 8;
const int colNum = 8;
//                    R1  R2  R3  R4  R5  R6  R7  R8
const int rowPins[] = {6, 11, A5,  9,  5, A4,  4, A2};
//                     C1  C2  C3  C4  C5  C6  C7  C8
const int colPins[] = {10,  3,  2,  7, A3,  8, 12, 13};

const int xPin = A1;
const int yPin = A0;

/******************** Variables Declarations ********************/

joystickReader joystick(xPin, yPin);
int row = 0;
int col = 0;

/****************** Function Declarations **********************/

void lightUp(int row, int col);

/*************************** Setup *****************************/

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < rowNum; i++) {
    pinMode(rowPins[i], OUTPUT);
    pinMode(colPins[i], OUTPUT);
  }

  for (int i = 0; i < rowNum; i++) {
    digitalWrite(rowPins[i], HIGH);
    digitalWrite(colPins[i], LOW);
  }

  joystick.pinInit();
}

/*************************** Loop *****************************/

void loop() {
  // put your main code here, to run repeatedly:
  util::pair<int, int> delta = joystick.readAxis();
  
  row = (row + delta.first + rowNum) % rowNum;
//  if (row < 0) { row = 0; }
//  else if (row >= 8) { row = 7; }
  
  col = (col + delta.second + colNum) % colNum;
//  if (col < 0) { col = 0; }
//  else if (col >= 8) { col = 7; }
  
  lightUp(row, col);
}

/****************** Function Definitions **********************/

void lightUp(int row, int col) {
  digitalWrite(rowPins[row], LOW);
  digitalWrite(colPins[col], HIGH);
  delay(1);
  digitalWrite(rowPins[row], HIGH);
  digitalWrite(colPins[col], LOW);
}
