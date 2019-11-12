#include "encoding.h"

/****************** Const Variables Declarations ******************/

// const int pinDP = 2;
// const int pinA = 3;
// const int pinB = 4;
// const int pinC = 5;
// const int pinD = 6;
// const int pinE = 7;
// const int pinF = 8;
// const int pinG = 9;

const int segmentsPins[noOfPins] = {2, 3, 4, 5, 6, 7, 8, 9};

/******************** Variables Declarations ********************/

/****************** Function Declarations **********************/

void displayDigit(int digit, byte decimalPoint = false);

/*************************** Setup *****************************/

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < noOfPins; i++) {
    pinMode(segmentsPins[i], OUTPUT);
  }
}

/*************************** Loop *****************************/

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 10; i++) {
    displayDigit(i);
    delay(500);
  }
}

/****************** Function Definitions **********************/

void displayDigit(int digit, byte decimalPoint) {
  for (int i = 0; i < noOfPins; i++) {
    digitalWrite(segmentsPins[i], displayEncoding[digit][i]);
  }
  digitalWrite(segmentsPins[0], decimalPoint);
}
