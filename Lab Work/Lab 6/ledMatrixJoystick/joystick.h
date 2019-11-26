#include "util.h"

const int thresholdLow = 400;
const int thresholdHigh = 600;

class joystickReader {
 private:
  const int pushbuttonPin_;
  const int xPin_;
  const int yPin_;
  int xValue_;
  int yValue_;
  int pushbuttonValue_;

 public:
  joystickReader(
    const int& xPin = 0,
    const int& yPin = 0,
    const int& pushbuttonPin = 0) :
      xPin_(xPin),
      yPin_(yPin),
      pushbuttonPin_(pushbuttonPin) {
        xValue_ = 0;
        yValue_ = 0;
        pushbuttonValue_ = 0;
  }
 
  int readPushbutton() {
    int currentButton = !digitalRead(pushbuttonPin_);
    if (currentButton == HIGH && pushbuttonValue_ == LOW) {
      pushbuttonValue_ = currentButton;
      return HIGH;
    } else {
      pushbuttonValue_ = currentButton;
      return LOW;
    }
  }

  util::pair<int, int> readAxis() {
    int currentXValue = analogRead(xPin_);
    if (currentXValue < thresholdLow) {
      currentXValue = -1;
    } else if (currentXValue > thresholdHigh) {
      currentXValue = 1;
    } else {
      currentXValue = 0;
    }
    if (currentXValue != xValue_) {
      xValue_ = currentXValue;
    } else {
      currentXValue = 0;
    }

    int currentYValue = analogRead(yPin_);
    if (currentYValue < thresholdLow) {
      currentYValue = -1;
    } else if (currentYValue > thresholdHigh) {
      currentYValue = 1;
    } else {
      currentYValue = 0;
    }
    if (currentYValue != yValue_) {
      yValue_ = currentYValue;
    } else {
      currentYValue = 0;
    }

    return {-currentXValue, currentYValue};
  }

  void pinInit() {
    pinMode(pushbuttonPin_, INPUT);
    pinMode(xPin_, INPUT);
    pinMode(yPin_, INPUT);
  }
};
