#include "util.h"
#include "globalState.h"
#include "matrixState.h"
#include <string.h>
#include <Wire.h>

struct baseModel {
  virtual void handleAction(action );
  virtual void redraw(bool redrawDisplay = false);
  virtual void init();
};

struct standby : baseModel {
  bool justExited = false;
  
  void handleAction(action a);
  void redraw(bool redrawDisplay);
  void init();
};

struct mainMenu : baseModel {
  enum type {StartGame, HighScore, Settings, Info, Exit, size};
  type cursor = StartGame;

  void handleAction(action a);
  void redraw(bool redrawDisplay);
  void init();
};

struct playMode : baseModel {
  enum type {StartGame, StartLevel, Play, NextLevel, Stop, size};
  type cursor = StartGame;

  const int messageDelay = 2000;
  const int bombsDelay = 1500;
  const int countDownStart = 3;
  const int diffChange = 5;
  int lastTime = 0;
  int countDown = 0;
  bool highScore;

  bool levelPassed;
  int lives;
  int level;
  int score;
  int difficulty;
  int diffCount;

  util::pair<int, int> currPos;
  util::pair<int, int> src;
  util::pair<int, int> dest;

  void handleAction(action a);
  void redraw(bool redrawDisplay);
  void init();
  void startGame();
  void stopGame();
  void loseLevel();
  void winLevel();
  void nextLevel();
  void startLevel(bool newLevel = true);
};

struct playMenu : baseModel {
  enum type {Resume, Exit, size};
  type cursor = Resume;
  
  void handleAction(action a);
  void redraw(bool redrawDisplay);
  void init();
};

struct highScoreMenu : baseModel {
  enum type {HighScore, Exit, size};
  type cursor = HighScore;

  void handleAction(action a);
  void redraw(bool redrawDisplay);
  void init();
  bool recordScore();
};

struct settingsMenu : baseModel {
  enum type {ChangeName, ChangeDifficulty, Exit, size};
  type cursor = ChangeName;

  void handleAction(action a);
  void redraw(bool redrawDisplay);
  void init();
};

struct changeName : baseModel {
  enum type {Name, Exit, size};
  type cursor = Name;

  bool editing;
  char current[nameLength + 1];
  int pos;
  
  void handleAction(action a);
  void redraw(bool redrawDisplay);
  void init();
};

struct changeDifficulty : baseModel {
  enum type {Difficulty, Exit, size};
  type cursor = Difficulty;

  bool editing;
  int current;

  void handleAction(action a);
  void redraw(bool redrawDisplay);
  void init();
};

struct infoMenu : baseModel {
  enum type {GameName, MyName, GithubLink, UnibucRobotics, Exit, size};
  type cursor = GameName;

  void handleAction(action a);
  void redraw(bool redrawDisplay);
  void init();
};

baseModel* model;
standby standbyInstance;
mainMenu mainMenuInstance;
playMenu playMenuInstance;
playMode playModeInstance;
highScoreMenu highScoreInstance;
settingsMenu settingsInstance;
changeName changeNameInstance;
changeDifficulty changeDifficultyInstance;
infoMenu infoMenuInstance;

void standby::handleAction(action a) {
  if (a.button == HIGH) {
    model = &mainMenuInstance;
    model->init();
  }
  justExited = false;
}

void standby::redraw(bool redrawDisplay) {
  if (!redrawDisplay) { return; }

  matrix.clear();
  lcd.clear();

  lcd.setCursor(0, 0);
  if (justExited) {
    lcd.print("Bye!");
    lcd.setCursor(0, 1);
    lcd.print("See you soon");
  } else {
    lcd.print("To play push");
    lcd.setCursor(0, 1);
    lcd.print("joystick button");
  }
}

void standby::init() {
  justExited = true;
}

void mainMenu::handleAction(action a) {
  if (a.button == HIGH) {
    if (cursor == StartGame) {
      model = &playModeInstance;
      playModeInstance.startGame();
    } else if (cursor == HighScore) {
      model = &highScoreInstance;
      model->init();
    } else if (cursor == Settings) {
      model = &settingsInstance;
      model->init();
    } else if (cursor == Info) {
      model = &infoMenuInstance;
      model->init();
    } else {
      cursor = StartGame;
      model = &standbyInstance;
      model->init();
    }
  } else if (a.x != 0) {
    cursor = (type)(((int)cursor + a.x + (int)size) % (int)size);
  }
}

void mainMenu::redraw(bool redrawDisplay) {
  matrix.clear();
  
  if (!redrawDisplay) { return; }
  
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Main Menu");
  lcd.setCursor(0, 1);
  lcd.print("> ");
  if (cursor == StartGame) {
    lcd.print("Start Game");
  } else if (cursor == HighScore) {
    lcd.print("High Score");
  } else if (cursor == Settings) {
    lcd.print("Settings");
  } else if (cursor == Info) {
    lcd.print("Info");
  } else {
    lcd.print("Exit");
  }
}

void mainMenu::init() {
  cursor = (type)0;
}

void playMode::handleAction(action a) {
  if (a.button == HIGH) {
    model = &playMenuInstance;
    model->init();
  } else if (cursor == StartGame || cursor == NextLevel) {
    int current = millis();
    if (current - lastTime > 1000) {
      countDown--;
      lastTime = current;
    }

    if (countDown == 0) {
      cursor = StartLevel;
      startLevel(levelPassed);
    }
  } else if (cursor == StartLevel) {
    int current = millis();
    if (current - lastTime > bombsDelay) {
      cursor = Play;
    }
  } else if (cursor == Stop) {
    int current = millis();
    if (current - lastTime > messageDelay) {
      model = &mainMenuInstance;
      model->init();
      model->redraw(true);
    }
  } else if (cursor == Play) {
    util::pair<int, int> prev = currPos;

    if (a.x != 0) {
      currPos.first += a.x;
      if (currPos.first < 0) {
        currPos.first = 0;
      } else if (currPos.first == 8) {
        currPos.first = 7;
      }
    } else {
      currPos.second += a.y;
      if (currPos.second < 0) {
        currPos.second = 0;
      } else if (currPos.second == 8) {
        currPos.second = 7;
      }
    }
    
    if (currPos == dest) {
      winLevel();
    } else if (matrix.board[currPos.first][currPos.second] == B) {
      loseLevel();
    } else {
      matrix.board[prev.first][prev.second] = P;
  
      if (matrix.path.size() > 1 && matrix.path[matrix.path.size() - 2] == currPos) {
        matrix.board[matrix.path.back().first][matrix.path.back().second] = F;
        matrix.path.pop_back();
      } else if (matrix.board[currPos.first][currPos.second] == P && currPos != prev) {
        currPos = prev;
      } else if (matrix.path.empty() || matrix.path.back() != currPos) {
        matrix.path.push_back(currPos);
      } 
  
      matrix.board[currPos.first][currPos.second] = C;
    }
  }
}

void playMode::redraw(bool redrawDisplay) {
  if (cursor == StartGame || cursor == NextLevel) {
    matrix.digit(countDown);
  } else if (cursor == StartLevel) {
    matrix.bombs();
  } else if (cursor == Stop) {
    matrix.clear();
  } else if (cursor == Play) {
    matrix.drawPath();
  }

  if (!redrawDisplay) { return; }
  
  lcd.clear();

  if (cursor == Stop) {
    if (highScore == true) {
      lcd.setCursor(0, 0);
      lcd.print("Whooo!");
      lcd.setCursor(0, 1);
      lcd.print("New highscore");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Too bad!");
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("lvl: ");
    lcd.print(level);
  
    lcd.setCursor(9, 0);
    lcd.print("diff: ");
    lcd.print(difficulty);
  
    lcd.setCursor(0, 1);
    lcd.print("score: ");
    lcd.print(score);
  
    lcd.createChar(0, heart);
    lcd.home();
    lcd.setCursor(14, 1);
    lcd.print(lives);
    lcd.write((uint8_t) 0);
  }
}

void playMode::init() {
  lives = initialLivesNo;
  difficulty = initialDifficulty;
  diffCount = 0;
  level = 0;
  levelPassed = true;
  score = 0;
}

void playMode::startGame() {
  init();
  cursor = StartGame;
  countDown = countDownStart;
  lastTime = millis();
}

void playMode::stopGame() {
  cursor = Stop;
  highScore = highScoreInstance.recordScore();
  init();
  lastTime = millis();
}

void playMode::loseLevel() {
  levelPassed = false;
  matrix.explode(currPos);
  lives--;
  if (lives == 0) {
    stopGame();
  } else {
    nextLevel();
  }
}

void playMode::winLevel() {
  levelPassed = true;
  score += matrix.computeScore(difficulty);
  level++;
  diffCount++;
  if (diffCount == diffChange && difficulty < 9) {
    difficulty++;
    diffCount = 0;
  }
  matrix.whole();
  nextLevel();
}

void playMode::nextLevel() {
  cursor = NextLevel;
  countDown = countDownStart;
  lastTime = millis();
}

void playMode::startLevel(bool newLevel) {
  if (newLevel) {
    matrix.createBoard(difficulty, diffCount);
  } else {
    matrix.resetBoard();
  }
  
  currPos = src = matrix.source();
  dest = matrix.destination();
  matrix.board[currPos.first][currPos.second] = C;
  lastTime = millis();
}

void playMenu::handleAction(action a) {
  if (a.button == HIGH) {
    if (cursor == Resume) {
      model = &playModeInstance;
    } else {
      model = &playModeInstance;
      playModeInstance.stopGame();
    }
  } else if (a.x != 0) {
    cursor = (type)(((int) cursor + a.x + (int)size) % (int)size);
  }
}

void playMenu::redraw(bool redrawDisplay) {
  if (!redrawDisplay) { return; }
  
  lcd.clear();
  matrix.pause();
  
  lcd.setCursor(2, 0);
  lcd.print("Resume");
  lcd.setCursor(2, 1);
  lcd.print("Quit");
  
  if (cursor == Resume) {
    lcd.setCursor(0, 0);
  } else {
    lcd.setCursor(0, 1);
  }
  lcd.print(">");
}

void playMenu::init() {
  cursor = (type)0;
}

void highScoreMenu::handleAction(action a) {
  if (a.button == HIGH) {
    if (cursor == Exit) {
      model = &mainMenuInstance;
    }
  } else if (a.x != 0) {
    cursor = (type)(((int) cursor + a.x + (int)size) % (int)size);
  }
}

void highScoreMenu::redraw(bool redrawDisplay) {
  if (!redrawDisplay) { return; }
  
  lcd.clear();
  matrix.clear();

  lcd.setCursor(0, 0);
  lcd.print("High Score");

  lcd.setCursor(0, 1);
  if (cursor == HighScore) {
    lcd.print(highScoreName);
    lcd.setCursor(nameLength + 1, 1);
    lcd.print(highScore);
  } else {
    lcd.print("> Back");
  }
}

void highScoreMenu::init() {
  cursor = (type)0;
}

bool highScoreMenu::recordScore() {
  if (playModeInstance.score > highScore) {
    highScore = playModeInstance.score;
    strcpy(highScoreName, name);
    return true;
  }
  return false;
}

void settingsMenu::handleAction(action a) {
  if (a.button == HIGH) {
    if (cursor == ChangeName) {
      model = &changeNameInstance;
      model->init();
    } else if (cursor == ChangeDifficulty) {
      model = &changeDifficultyInstance;
      model->init();
    } else {
      model = &mainMenuInstance;
    }
  } else if (a.x != 0) {
    cursor = (type)(((int) cursor + a.x + (int)size) % (int)size);
  }
}

void settingsMenu::redraw(bool redrawDisplay) {
  if (!redrawDisplay) { return; }
  
  lcd.clear();
  matrix.clear();

  lcd.setCursor(0, 0);
  lcd.print("Settings");

  lcd.setCursor(0, 1);
  lcd.print("> ");
  if (cursor == ChangeName) {
    lcd.print("Set Name");
  } else if (cursor == ChangeDifficulty) {
    lcd.print("Set Difficulty");
  } else {
    lcd.print("Back");
  }
}

void settingsMenu::init() {
  cursor = (type)0;
}

void changeName::handleAction(action a) {
  if (a.button == HIGH) {
    if (editing == true) {
      editing = false;
    } else if (cursor == Name) {
      editing = true;
      pos = 0;
    } else {
      strcpy(name, current);
      model = &settingsInstance;
    }
  } else {
    if (editing == false) {
      if (a.x != 0) {
        cursor = (type)(((int) cursor + a.x + (int)size) % (int)size);
      }
    } else if (a.x != 0) {
      if (current[pos] == ' ') {
        if (a.x > 0) {
          current[pos] = 'z';
        } else {
          current[pos] = 'a';
        }
      } else {
        current[pos] -= a.x;
        if (current[pos] < 'a' || current[pos] > 'z') {
          current[pos] = ' ';
        }
      }
    } else if (a.y != 0) {
      pos = (pos + a.y + nameLength) % nameLength;
      if (current[pos] == '\0') {
        current[pos] = ' ';
        current[pos + 1] = '\0';
      }
    }
  }
}

void changeName::redraw(bool redrawDisplay) {
  if (!redrawDisplay) { return; }
   
  lcd.clear();
  matrix.clear();

  if (editing == false) {
    lcd.setCursor(2, 0);
    lcd.print("Edit Name");
    lcd.setCursor(2, 1);
    lcd.print("Back");
    if (cursor == Name) {
      lcd.setCursor(0, 0);
    } else {
      lcd.setCursor(0, 1);
    }
    lcd.print("> ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Name:");
    lcd.setCursor(0, 1);
    lcd.print(current);
    lcd.setCursor(pos, 1);
    if (current[pos] == ' ') {
      lcd.print('_');
    } else {
      lcd.print((char)(current[pos] - 'a' + 'A'));
    }
  }
}

void changeName::init() {
  cursor = (type)0;
  strcpy(current, name);
  pos = 0;
}

void changeDifficulty::handleAction(action a) {
  if (a.button == HIGH) {
    if (editing) {
      editing = false;
    } else if (cursor == Difficulty) {
      editing = true;
    } else {
      initialDifficulty = current;
      model = &settingsInstance;
    }
  } else if (a.x != 0) {
    if (editing) {
      current = (current - a.x + maxDifficulty) % maxDifficulty;
    } else {
      cursor = (type)(((int) cursor + a.x + (int)size) % (int)size);
    }
  }
}

void changeDifficulty::redraw(bool redrawDisplay) {
  if (!redrawDisplay) { return; }
  
  matrix.clear();
  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.print("Difficulty: ");
  lcd.print(current);
  lcd.setCursor(2, 1);
  lcd.print("Back");

  if (editing == false) {
    if (cursor == Difficulty) {
      lcd.setCursor(0, 0);
    } else {
      lcd.setCursor(0, 1);
    }
    lcd.print("> ");
  }
}

void changeDifficulty::init() {
  cursor = (type)0;
  current = initialDifficulty;
}

void infoMenu::handleAction(action a) {
  if (a.button == HIGH) {
    if (cursor == Exit) {
      model = &mainMenuInstance;
    }
  } else {
    cursor = (type)(((int) cursor + a.x + (int)size) % (int)size);
  }
}

void infoMenu::redraw(bool redrawDisplay) {
  if (!redrawDisplay) { return; }

  matrix.clear();
  lcd.clear();

  lcd.setCursor(0, 0);
  
  if (cursor == GameName) {
    lcd.print("Game:");
    lcd.setCursor(2, 1);
    lcd.print("Perilious Path");
  } else if (cursor == MyName) {
    lcd.print("Created by:");
    lcd.setCursor(3, 1);
    lcd.print("Livia");
  } else if (cursor == GithubLink) {
    lcd.print("github.com/");
    lcd.setCursor(8, 1);
    lcd.print("Oepeling");
  } else if (cursor == UnibucRobotics) {
    lcd.print("At:");
    lcd.setCursor(1, 1);
    lcd.print("@UnibucRobotics");
  } else if (cursor == Exit) {
    lcd.print("> Back");
  }
}

void infoMenu::init() {
  cursor = (type)0;
}
