#pragma once

#include "globalState.h"
#include "LedControl.h"
#include "level.h"
#include "util.h"
#include "vector.h"

LedControl lc(12, 11, 10, 1);

uint8_t digits[10][8] = {
  {
    B00000000,
    B00111100,
    B00100100,
    B00100100,
    B00100100,
    B00100100,
    B00100100,
    B00111100
  },
  {
    B00000000,
    B00000100,
    B00000100,
    B00000100,
    B00000100,
    B00000100,
    B00000100,
    B00000100
  },
  {
    B00000000,
    B00111100,
    B00000100,
    B00000100,
    B00111100,
    B00100000,
    B00100000,
    B00111100
  },
  {
    B00000000,
    B00111100,
    B00000100,
    B00000100,
    B00111100,
    B00000100,
    B00000100,
    B00111100
  },
  {
    B00000000,
    B00100100,
    B00100100,
    B00100100,
    B00111100,
    B00000100,
    B00000100,
    B00000100
  },
  {
    B00000000,
    B00111100,
    B00100000,
    B00100000,
    B00111100,
    B00000100,
    B00000100,
    B00111100
  },
  {
    B00000000,
    B00111100,
    B00100000,
    B00100000,
    B00111100,
    B00100100,
    B00100100,
    B00111100
  },
  {
    B00000000,
    B00111100,
    B00000100,
    B00000100,
    B00000100,
    B00000100,
    B00000100,
    B00000100
  },
  {
    B00000000,
    B00111100,
    B00100100,
    B00100100,
    B00111100,
    B00100100,
    B00100100,
    B00111100
  },
  {
    B00000000,
    B00111100,
    B00100100,
    B00100100,
    B00111100,
    B00000100,
    B00000100,
    B00111100
  }
};

struct matrixState {
  const int blinkTime = 100;

  int lastBlinkTime = 0;
  bool blinkState = false;

  boardType original[8][8];
  boardType board[8][8];
  vector<util::pair<int, int>> path;
  int shortest = 0;

  util::pair<int, int> source();
  util::pair<int, int> destination();
  void createBoard(int difficulty, int level);
  void resetBoard();
  int lee(util::pair<int, int> src, util::pair<int, int> dest);
  int computeScore(int difficulty);
  
  void explode(util::pair<int, int> pos);
  void whole();
  void bombs();
  void drawPath();
  void clear();
  void pause();
  void digit(int d);
};

matrixState matrix;

int matrixState::lee(util::pair<int, int> src, util::pair<int, int> dest) {
  int dist[8][8];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      dist[i][j] = 0;
      if (board[i][j] == B) {
        dist[i][j] = -1;
      }
    }
  }

  dist[src.first][src.second] = 1;
  
  for (int d = 1; d <= 64; d++) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (dist[i][j] != d) { continue; }
        if (i != 0 && dist[i - 1][j] == 0) {
          dist[i - 1][j] = d + 1;
        }
        if (i != 7 && dist[i + 1][j] == 0) {
          dist[i + 1][j] = d + 1;
        }
        if (j != 0 && dist[i][j - 1] == 0) {
          dist[i][j - 1] = d + 1;
        }
        if (j != 7 && dist[i][j + 1] == 0) {
          dist[i][j + 1] = d + 1;
        }
      }
    }
  }

  return (dist[dest.first][dest.second] - 1);
}

void matrixState::createBoard(int difficulty, int level) {
  Generate(2 * difficulty + level / 3 - 1);
  
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      original[i][j] = board[i][j] = (boardType) (genBoard[i][j]);
    }
  }

  path.erase();
  shortest = lee(source(), destination());
}

void matrixState::resetBoard() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = original[i][j];
    }
  }

  path.erase();
}

util::pair<int, int> matrixState::source() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (original[i][j] == S) {
        return {i, j};
      }
    }
  }
}

util::pair<int, int> matrixState::destination() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (original[i][j] == D) {
        return {i, j};
      }
    }
  }
}

int matrixState::computeScore(int difficulty) {
  difficulty++;
  if (path.size() == shortest) {
    return 15.0 * difficulty;
  } else {
    return (10.0 * difficulty * (1.0 * shortest / path.size()));
  }
}

void matrixState::explode(util::pair<int, int> pos) {
  auto draw = [&](int l, bool state) {
    for (int row = 0; row <= l; row ++) {
      int col = l - row;
      for (int sr = -1; sr <= 1; sr += 2) {
        for (int sc = -1; sc <= 1; sc += 2) {
          if (pos.first + sr * row < 8 && pos.first + sr * row >= 0 &&
              pos.second + sc * col < 8 && pos.second + sc * col >= 0) {
            lc.setLed(0, pos.first + sr * row, pos.second + sc * col, state);
          }
        }
      }
    }
  };

  clear();
  draw(0, true);
  delay(50);
  for (int i = 0; i < 15; i++) {
    draw(i + 1, true);
    delay(30);
    draw(i, false);
    delay(30);
  }
}

void matrixState::whole() {
  clear();

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (board[row][col] == P || board[row][col] == C || board[row][col] == D) {
        lc.setLed(0, row, col, true);
      }
    }
  }

  int startTime = millis();
  int current = startTime;
  lastBlinkTime = startTime;
  blinkState = false;
  
  while(current - startTime < 2000) {
    if (current - lastBlinkTime > blinkTime) {
      blinkState ^= 1;
      lastBlinkTime = current;
      for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
          if (board[row][col] == B) {
            lc.setLed(0, row, col, blinkState);
          }
        }
      }
    }
    current = millis();
  }
}

void matrixState::bombs() {
  clear();

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (board[row][col] == B) {
        lc.setLed(0, row, col, true);
      }
    }
  }
}

void matrixState::drawPath() {
  clear();
  
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (board[row][col] == P || board[row][col] == D) {
        lc.setLed(0, row, col, true);
      } else if (board[row][col] == C) {
        int current = millis();
        if (current - lastBlinkTime > blinkTime) {
          blinkState ^= 1;
          lastBlinkTime = current;
        }
        lc.setLed(0, row, col, blinkState);
      }
    }
  }
}

void matrixState::clear() {
  lc.clearDisplay(0);
}

void matrixState::pause() {
  clear();

  for (int row = 0; row < 8; row++) {
    lc.setLed(0, row, 1, true);
    lc.setLed(0, row, 2, true);
    lc.setLed(0, row, 5, true);
    lc.setLed(0, row, 6, true);
  }
}

void matrixState::digit(int d) {
  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, digits[d][row]);
  }
}
