#include "LedControl.h"

LedControl lc(12, 11, 10, 1);  // DIN, CLK, LOAD, No DRIVER

// Adruino    -> MAX7219
// Ard pin 12 -> driver pin 1
// Ard pin 11 -> CLK pin 13
// Ard pin 10 -> LOAD pin 12
// 1 as we are only using one driver

void setup() {
  // put your setup code here, to run once:
  lc.shutdown(0, false);  // turn off power saver, enable display
  lc.setIntensity(0, 2);  // sets brightness (0 ~ 15 possible values)
  lc.clearDisplay(0);  // clear screen
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      lc.setLed(0, col, row, true);  // turns on led on col, row
      delay(25);
    }
  }
}
