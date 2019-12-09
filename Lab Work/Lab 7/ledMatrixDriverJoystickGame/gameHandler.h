#include "util.h"
#include "LedControl.h"

const bool prototype[8][8] = {
  {0, 0, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 1, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 0}
};

class gameHandler {
private:
  const int rowsNo_;
  const int colsNo_;
  const int blinkDelay_;
  const bool gameBoard_[8][8];
  int row_, col_;
  int blinkTime_;
  bool blinkOn_;

public:
  gameHandler(
    const int& rows = 8,
    const int& cols = 8) :
      rowsNo_(rows),
      colsNo_(cols),
      gameBoard_(prototype),
      blinkDelay_(100),
      row_(0),
      col_(0), 
      blinkTime_(0),
      blinkOn_(false) {}

  void handleAction(const action& a) {
    row_ = max(min(row_ - a.x, rowsNo_ - 1), 0);
    col_ = max(min(col_ + a.y, colsNo_ - 1), 0);

    if (gameBoard_[row_][col_] == true) {
      row_ = col_ = 0;
    }
  }

  void redraw(LedControl& lc) {
    for (int i = 0; i < rowsNo_; i++) {
      for (int j = 0; j < colsNo_; j++) {
        if (gameBoard_[i][j]) {
          lc.setLed(0, i, j, true);
          delay(1);
          lc.setLed(0, i, j, false);
        }
      }
    }

    int currentTime = millis();
    if (currentTime - blinkTime_ > blinkDelay_) {
      blinkOn_ ^= 1;
      blinkTime_ = currentTime;
    }

    if (blinkOn_) {
      lc.setLed(0, row_, col_, true);
      delay(1);
      lc.setLed(0, row_, col_, false);
    }
  }
};
