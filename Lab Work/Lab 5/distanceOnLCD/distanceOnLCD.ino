#include <LiquidCrystal.h>

/****************** Const Variables Declarations ******************/

const int trigPin = 9;
const int echoPin = 10;

/******************** Variables Declarations ********************/

LiquidCrystal lcd(12, 11, 2, 3, 4, 5);
int duration = 0;
int distance = 0;

/****************** Function Declarations **********************/

/*************************** Setup *****************************/

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

/*************************** Loop *****************************/

void loop() {
  // put your main code here, to run repeatedly:
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);

  // Sets the trigPin to HIGH for 10ms
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  // Reads the echoPin, returns the sound wave travel time
  duration = pulseIn(echoPin, HIGH, 1e4);

  // Computing distance
  distance = duration * 0.034 / 2;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  delay(100);
}

/****************** Function Definitions **********************/
