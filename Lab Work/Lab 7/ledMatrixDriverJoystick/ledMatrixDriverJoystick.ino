#include "LedControl.h"
#include "joystickReader.h"
#include "util.h"

const int rowsNo = 8;
const int colsNo = 8;

LedControl lc(12, 11, 10, 1);  // DIN, CLK, LOAD, No DRIVER
joystickReader joystick(8, A0, A1);

int row = 0;
int col = 0;

void circularMove(int& row, int& col);
void move(int& row, int& col);

void setup() {
  // put your setup code here, to run once:
  lc.shutdown(0, false);  // turn off power saver, enable display
  lc.setIntensity(0, 2);  // sets brightness (0 ~ 15 possible values)
  lc.clearDisplay(0);  // clear screen
  joystick.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  circularMove(row, col);
  lc.setLed(0, row, col, true);
  delay(1);
  lc.setLed(0, row, col, false);
}

void circularMove(int& row, int& col) {
  util::pair<int, int> axis = joystick.readAxis();
  row = (row - axis.first + rowsNo) % rowsNo;
  col = (col + axis.second + colsNo) % colsNo;
}

void move(int& row, int& col) {
  util::pair<int, int> axis = joystick.readAxis();
  row = max(min(row - axis.first, rowsNo - 1), 0);
  col = max(min(col + axis.second, colsNo - 1), 0);
}
