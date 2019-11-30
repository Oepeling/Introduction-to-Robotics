#include "joystickReader.h"
#include "gameState.h"
#include "globalState.h"
#include "util.h"

/**************** Const Variables Declarations ******************/

/******************** Variables Declarations ********************/

joystickReader joystick(7, A0, A1);

/****************** Function Declarations **********************/

void handleAction(action a) {
  model->handleAction(a);
  model->redraw();
}

/*************************** Setup *****************************/

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  joystick.begin();
  model = &standbyInstance;
  model->redraw();
}

/*************************** Loop *****************************/

void loop() {
  // put your main code here, to run repeatedly:
  if (joystick.readPushbutton() == HIGH) {
    handleAction(action({HIGH, 0, 0}));
  } else if (joystick.readAxis() != util::pair<int, int>({0, 0})) {
    handleAction(action({LOW, joystick.x(), joystick.y()}));
  }
}

/****************** Function Definitions **********************/
