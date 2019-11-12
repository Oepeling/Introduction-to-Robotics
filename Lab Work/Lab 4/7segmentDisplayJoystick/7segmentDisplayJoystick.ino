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

const int switchPin = 12; // digital pin connected to switch output
const int xPin = A0; // A0 - analog pin connected to X output
const int yPin = A1; // A1 - analog pin connected to Y output

const int xLow = 300;
const int xHigh = 700;

/******************** Variables Declarations ********************/

int switchValue;
int xValue = 0;
int yValue = 0;

int currentDigit = 0;
byte decimalPoint = false;

/****************** Function Declarations **********************/

void displayDigit(int digit, byte decimalPoint = false);
bool shouldSwitch();
bool xChanged();

/*************************** Setup *****************************/

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < noOfPins; i++) {
    pinMode(segmentsPins[i], OUTPUT);
  }
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(switchPin, INPUT_PULLUP); //activate pull-up resistor on the 
                                // push-button pin
  Serial.begin(9600);
}

/*************************** Loop *****************************/

void loop() {
  // put your main code here, to run repeatedly:
  if (shouldSwitch()) {
    Serial.println("It made it to if");
    decimalPoint = (!decimalPoint);
  } else if (xChanged()) {
    currentDigit = (currentDigit + xValue + 10) % 10;
  }

  displayDigit(currentDigit, decimalPoint);
  delay(10);
}

/****************** Function Definitions **********************/

void displayDigit(int digit, byte decimalPoint) {
  for (int i = 0; i < noOfPins; i++) {
    digitalWrite(segmentsPins[i], displayEncoding[digit][i]);
  }
  digitalWrite(segmentsPins[0], decimalPoint);
}

bool shouldSwitch() {
  int newSwitchValue = !digitalRead(switchPin);
  
  if (newSwitchValue == HIGH && switchValue == LOW) {
    switchValue = newSwitchValue;
    return true;
  } else {
    switchValue = newSwitchValue;
    return false;
  }
}

bool xChanged() {
  int newXValue = analogRead(xPin);
  
  if (newXValue < xLow) {
    newXValue = -1;
  } else if (newXValue > xHigh) {
    newXValue = 1;
  } else {
    newXValue = 0;
  }

  if (newXValue != xValue) {
    xValue = newXValue;
    return true;
  } else {
    return false;
  }
}
