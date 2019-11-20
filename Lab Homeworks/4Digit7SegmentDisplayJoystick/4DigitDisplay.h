#include "util.h"

const int noOfDigits = 4;
const int noOfSegments = 7;
const int sigma = 10;

byte displayEncoding[sigma][noOfSegments] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

class With4DigitDisplay {
private:
  const int digitPins_[noOfDigits];
  const int segmentPins_[noOfSegments];
  const int DPPin_;
  const int blinkDelay_;
  int digits_[noOfDigits];
  int pointer_;
  bool digitChange_;
  int lastUpdateDP_;
  bool lastDPState_;
  
public:
  With4DigitDisplay(
    const int& pinD1 = 0, const int& pinD2 = 0, const int& pinD3 = 0,
    const int& pinD4 = 0, const int& pinDP = 0, const int& pinA = 0,
    const int& pinB = 0, const int& pinC = 0, const int& pinD = 0,
    const int& pinE = 0, const int& pinF = 0, const int& pinG = 0) :
      digitPins_({pinD1, pinD2, pinD3, pinD4}),
      segmentPins_({pinA, pinB, pinC,
                    pinD, pinE, pinF, pinG}),
      DPPin_(pinDP),
      blinkDelay_(100) {
        digits_[0] = digits_[1] = 
        digits_[2] = digits_[3] = 0;
        pointer_ = 0;
        digitChange_ = false;
        lastUpdateDP_ = 0;
        lastDPState_ = false;
  }

  void print() {
    for (int i = 0; i < noOfDigits; i++) {
      digitalWrite(digitPins_[i], HIGH);
    }
    for (int i = 0; i < noOfDigits; i++) {
      for (int j = 0; j < noOfSegments; j++) {
        digitalWrite(
          segmentPins_[j],
          displayEncoding[digits_[i]][j]);
      }

      if (pointer_ == i) {
        if (digitChange_ == false) {
          digitalWrite(DPPin_, HIGH);
        } else {
          int currentTime = millis();
          if (currentTime - lastUpdateDP_ > blinkDelay_) {
            lastDPState_ = !lastDPState_;
            digitalWrite(DPPin_, lastDPState_);
            lastUpdateDP_ = currentTime;
          }
        }
      }
      
      digitalWrite(digitPins_[i], LOW); 
      delay(1);
      digitalWrite(digitPins_[i], HIGH);
    }
  }

  void selectDigit() {
    digitChange_ = !digitChange_;
  }

  bool isSelected() {
    return digitChange_;
  }

  void movePointer(int value) {
    if (isSelected()) return;
    pointer_ = (pointer_ + value + noOfDigits) % noOfDigits;
  }

  void changeDigit(int value) {
    if (!isSelected()) return;
    digits_[pointer_] = (digits_[pointer_] + value + sigma) % sigma;
  }

  void update(util::pair<int, int> value) {
    if (isSelected()) {
      changeDigit(value.second);
    } else {
      movePointer(value.first);
    }
  }

  void pinInit() {
    for (int i = 0; i < noOfDigits; i++) {
      pinMode(digitPins_[i], OUTPUT);
    }
    for (int i = 0; i < noOfSegments; i++) {
      pinMode(segmentPins_[i], OUTPUT);
    }
  }
};
