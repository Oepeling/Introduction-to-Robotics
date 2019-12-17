#pragma once

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 2, 3, 4, 5);

uint8_t heart[] = {
  B00000,
  B00000,
  B01010,
  B11101,
  B11111,
  B11111,
  B01110,
  B00100
};

int initialDifficulty = 0;
const int maxDifficulty = 10;
int initialLivesNo = 5;
const int nameLength = 8;
char name[nameLength + 1] = "anon    ";

int highScore = 0;
char highScoreName[nameLength + 1] = "anon    ";
