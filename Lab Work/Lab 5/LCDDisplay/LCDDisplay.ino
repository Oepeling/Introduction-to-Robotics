#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("hello, world!");
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(6, 0);
  lcd.print(millis() / 1000);
}