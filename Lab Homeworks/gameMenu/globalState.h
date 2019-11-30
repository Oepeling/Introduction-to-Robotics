#pragma once

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int initialLevel = 0;
const int maxLevel = 100;
int initialLivesNo = 3;
const int nameLength = 8;
char name[nameLength + 1] = "anon    ";

int highScore = 0;
char highScoreName[nameLength + 1] = "anon    ";
