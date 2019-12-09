#include "LedControl.h"
#include "gameHandler.h"
#include "joystickReader.h"
#include "util.h"

const int rowsNo = 8;
const int colsNo = 8;

LedControl lc(12, 11, 10, 1);  // DIN, CLK, LOAD, No DRIVER
joystickReader joystick(8, A0, A1);
gameHandler game;

void setup() {
  // put your setup code here, to run once:
  lc.shutdown(0, false);  // turn off power saver, enable display
  lc.setIntensity(0, 2);  // sets brightness (0 ~ 15 possible values)
  lc.clearDisplay(0);  // clear screen
  joystick.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  util::pair<int, int> axis = joystick.readAxis();
  game.handleAction({axis.first, axis.second, LOW});
  game.redraw(lc);
  delay(1);
}
