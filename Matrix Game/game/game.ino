#include "joystickReader.h"
#include "menuState.h"
#include "globalState.h"
#include "matrixState.h"
#include "util.h"

#include <stdlib.h>

/**************** Const Variables Declarations ******************/

/******************** Variables Declarations ********************/

joystickReader joystick(7, A1, A0);

/****************** Function Declarations **********************/

void handleAction(action a) {
  model->handleAction(a);
  model->redraw((a != action({LOW, 0, 0})));
}

/*************************** Setup *****************************/

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  joystick.begin();
  lc.shutdown(0, false);
  lc.setIntensity(0, 0);
  lc.clearDisplay(0);
  srand(millis());
  
  model = &standbyInstance;
  model->redraw(true);
}

/*************************** Loop *****************************/

void loop() {
  // put your main code here, to run repeatedly:
//  matrix.pause();
//  delay(1000);
//  matrix.explode({3, 4});
//  delay(1000);

  action a;
  if (joystick.readPushbutton() == HIGH) {
    a = {HIGH, 0, 0};
  } else if (joystick.readAxis() != util::pair<int, int>({0, 0})) {
    a = {LOW, joystick.x(), joystick.y()};
  } else {
    a = {LOW, 0, 0};
  }

  handleAction(a);
  delay(1);
}

/****************** Function Definitions **********************/
