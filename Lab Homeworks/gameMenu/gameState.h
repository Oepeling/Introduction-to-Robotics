#include "util.h"
#include "globalState.h"
#include <string.h>

struct baseModel {
  virtual void handleAction(action );
  virtual void redraw();
};

struct standby : baseModel {
  bool justExited = false;
  
  void handleAction(action a);
  void redraw();
  void init();
};

struct mainMenu : baseModel {
  enum type {StartGame, HighScore, Settings, Exit, size};
  type cursor = StartGame;

  void handleAction(action a);
  void redraw();
  void init();
};

struct playMode : baseModel {
  int lives;
  int level;
  int score;
  
  void handleAction(action a);
  void redraw();
  void reset();
  void recordScore();
};

struct playMenu : baseModel {
  enum type {Resume, Exit, size};
  type cursor = Resume;
  
  void handleAction(action a);
  void redraw();
  void init();
};

struct highScoreMenu : baseModel {
  enum type {HighScore, Exit, size};
  type cursor = HighScore;

  void handleAction(action a);
  void redraw();
  void init();
};

struct settingsMenu : baseModel {
  enum type {ChangeName, ChangeLevel, Exit, size};
  type cursor = ChangeName;

  void handleAction(action a);
  void redraw();
  void init();
};

struct changeName : baseModel {
  enum type {Name, Exit, size};
  type cursor = Name;

  bool editing;
  char current[nameLength + 1];
  int pos;
  
  void handleAction(action a);
  void redraw();
  void init();
};

struct changeLevel : baseModel {
  enum type {Level, Exit, size};
  type cursor = Level;

  bool editing;
  int current;

  void handleAction(action a);
  void redraw();
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
changeLevel changeLevelInstance;

void standby::handleAction(action a) {
  if (a.button == HIGH) {
    model = &mainMenuInstance;
    mainMenuInstance.init();
  }
  justExited = false;
}

void standby::redraw() {
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
      playMenuInstance.init();
      playModeInstance.reset();
    } else if (cursor == HighScore) {
      model = &highScoreInstance;
      highScoreInstance.init();
    } else if (cursor == Settings) {
      model = &settingsInstance;
      settingsInstance.init();
    } else {
      model = &standbyInstance;
      standbyInstance.init();
    }
  } else if (a.x != 0) {
    cursor = (type)(((int)cursor + a.x + (int)size) % (int)size);
  }
}

void mainMenu::redraw() {
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
    playMenuInstance.init();
  }
}

void playMode::redraw() {
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("lvl: ");
  lcd.print(level);

  lcd.setCursor(8, 0);
  lcd.print("lives: ");
  lcd.print(lives);

  lcd.setCursor(0, 1);
  lcd.print("score: ");
  lcd.print(score);
}

void playMode::reset() {
  lives = initialLivesNo;
  level = initialLevel;
  score = level * 3;
}

void playMode::recordScore() {
  if (score > highScore) {
    highScore = score;
    strcpy(highScoreName, name);
  }
}

void playMenu::handleAction(action a) {
  if (a.button == HIGH) {
    if (cursor == Resume) {
      model = &playModeInstance;
    } else {
      model = &mainMenuInstance;
      playModeInstance.recordScore();
    }
  } else if (a.x != 0) {
    cursor = (type)(((int) cursor + a.x + (int)size) % (int)size);
  }
}

void playMenu::redraw() {
  lcd.clear();
  
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

void highScoreMenu::redraw() {
  lcd.clear();

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

void settingsMenu::handleAction(action a) {
  if (a.button == HIGH) {
    if (cursor == ChangeName) {
      model = &changeNameInstance;
      changeNameInstance.init();
    } else if (cursor == ChangeLevel) {
      model = &changeLevelInstance;
      changeLevelInstance.init();
    } else {
      model = &mainMenuInstance;
    }
  } else if (a.x != 0) {
    cursor = (type)(((int) cursor + a.x + (int)size) % (int)size);
  }
}

void settingsMenu::redraw() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Settings");

  lcd.setCursor(0, 1);
  lcd.print("> ");
  if (cursor == ChangeName) {
    lcd.print("Set Name");
  } else if (cursor == ChangeLevel) {
    lcd.print("Set Level");
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

void changeName::redraw() {
  lcd.clear();

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

void changeLevel::handleAction(action a) {
  if (a.button == HIGH) {
    if (editing) {
      editing = false;
    } else if (cursor == Level) {
      editing = true;
    } else {
      initialLevel = current;
      model = &settingsInstance;
    }
  } else if (a.x != 0) {
    if (editing) {
      current = (current - a.x + maxLevel) % maxLevel;
    } else {
      cursor = (type)(((int) cursor + a.x + (int)size) % (int)size);
    }
  }
}

void changeLevel::redraw() {
  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.print("Level: ");
  lcd.print(current);
  lcd.setCursor(2, 1);
  lcd.print("Back");

  if (editing == false) {
    if (cursor == Level) {
      lcd.setCursor(0, 0);
    } else {
      lcd.setCursor(0, 1);
    }
    lcd.print("> ");
  }
}

void changeLevel::init() {
  cursor = (type)0;
  current = initialLevel;
}
