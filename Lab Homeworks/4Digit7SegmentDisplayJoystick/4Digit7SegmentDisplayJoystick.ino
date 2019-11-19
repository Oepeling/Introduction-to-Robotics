#include "joystickReader.h"
#include "4DigitDisplay.h"

/****************** Const Variables Declarations ******************/

const int pinDP = 4;
const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;

const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int switchPin = A2;
const int xPin = A0;
const int yPin = A1;

/******************** Variables Declarations ********************/

joystickReader joystick(switchPin, xPin, yPin);
With4DigitDisplay display(
  pinD1, pinD2, pinD3, pinD4,
  pinDP, pinA, pinB, pinC,
  pinD, pinE, pinF, pinG); 

/****************** Function Declarations **********************/

/*************************** Setup *****************************/

void setup() {
  // put your setup code here, to run once:
  joystick.pinInit();
  display.pinInit();
}

/*************************** Loop *****************************/

void loop() {
  // put your main code here, to run repeatedly:
  if (joystick.readPushbutton() == HIGH) {
    display.selectDigit();
  } else {
    display.update(joystick.readAxis());
  }
  display.print();
  delay(5);
}

/****************** Function Definitions **********************/
