#include "pitches.h"

const int melody[] = {
  NA4, NC5, NE5, NA5, NB5, NE5, NC5, NB5, 
  NC6, NE5, NC5, NC6, NFS5, ND5, NA4, NFS5, 
  NE5, NC5, NA4, NC5, 0, NE5, NC5, NA4, 
  NG4, NA4, NA4, 0, 0, NA4, NF5, NE5,

  NA4, NA4, NC5, NE5, NB5, NE5, NC5, NB5, 
  NC6, NE5, NC5, NC6, NFS5, ND5, NA4, NFS5, 
  NE5, NC5, NA4, NC5, 0, NE5, NC5, NA4, 
  NG4, NA4, NA4, 0, 0, 0, NA4, NB4,

  NC5, NE5, NG5, NC5, NB4, ND5, NG5, NB4, 
  NB4, NC5, NA4, NA4, NC5, NE5, NA5, NA4, 
  NB4, NC5, NG4, NC4, NC5, NG5, ND5, NG4, 
  NG5, NG5, NFS5, ND5, ND5, 0, 0, NA4, NB4, 

  NC5, NE5, NG5, NC5, NB4, ND5, NG5, NB4, 
  NB4, NC5, NA4, NA4, NC5, NE5, NA5, NA4, NB4, 
  NC5, NE5, NG5, NC6, ND5, NFS5, NA5, 
  ND6, NE5, NE5, NE5, 0, 0,
};

const int tempo[] = {
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 4, 12, 8, 8, 8,
8, 8, 8, 8, 4, 8, 8, 8,

8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 4, 12, 8, 8, 8,
8, 8, 8, 8, 4, 8, 8, 8,

8, 8, 8, 8, 8, 8, 8, 8,
16, 16, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8,
16, 16, 8, 4, 12, 8, 8, 8,

8, 8, 8, 8, 8, 8, 8, 8,
16, 16, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 2, 4, 4,
};

const int activeBuzzerPin = 5;
const int pasiveBuzzerPin = A0;
const int pushbuttonPin = 4;
const unsigned long delayTime = 5000;

bool knockDetected = false;
bool buttonPushed = false;
bool songPlaying = false;
unsigned long knockTime = 0;

int knockValue = 0;
const int threshold = 70;

int currentNote = 0;
enum playingState { PAUSE, PLAYING };
playingState currentState = PAUSE;
unsigned long lastUpdateTime = 0;
unsigned long noteDuration = 0;
unsigned long delayValue = 0;
int numCycles = 0;
int currentCycle = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(activeBuzzerPin, OUTPUT);
  pinMode(pasiveBuzzerPin, INPUT);
  pinMode(pushbuttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!knockDetected && knockDetection) { // knock detection
    knockDetected = true;
    buttonPushed = false;
    knockTime = millis();
  } else if (knockDetected && !buttonPushed && millis() - knockTime > delayTime && !songPlaying) {
    startPlayingSong();
  } else if (knockDetected && digitalRead(pushbuttonPin) == HIGH && songPlaying) {
    buttonPushed = true;
  } else if (knockDetected && buttonPushed && songPlaying) {
    stopPlayingSong();
    knockDetected = false;
    buttonPushed = false;
  } else if (songPlaying) {
    continuePlayingSong();
  }

  delay(1);
}

bool knockDetection() {
  knockValue = analogRead(pasiveBuzzerPin);
  if (knockValue > threshold) {
    Serial.println("Knock detected");
    return true;
  } else {
    return false;
  }
}

int getSize(const int* vector) {
  return sizeof(vector) / sizeof(int);
}

void startPlayingSong() {
  Serial.println("Starway to Heaven");
  songPlaying = true;
  
  currentNote = 0;
  currentState = PLAYING;
  noteDuration = 1200 / tempo[currentNote];
  delayValue = 1000000 / melody[currentNote] / 2;
  numCycles = melody[currentNote] * noteDuration / 1000;
  currentCycle = 0;

  digitalWrite(activeBuzzerPin, HIGH);
  lastUpdateTime = millis();
}

void continuePlayingSong() {
  if (millis() - lastUpdateTime < delayValue) {
    return;
  } else if (currentState == PLAYING) {
    currentState = PAUSE;
    
    digitalWrite(activeBuzzerPin, LOW);
    lastUpdateTime = millis();
  } else {
    currentCycle++;
    
    if (currentCycle == numCycles) {
      delayValue = noteDuration * 1.3;
      currentState = PAUSE;

      digitalWrite(activeBuzzerPin, LOW);
      lastUpdateTime = millis();
    } else if (currentCycle > numCycles) {
      currentNote = (currentNote + 1) % getSize(melody);
      currentState = PLAYING;
      noteDuration = 1200 / tempo[currentNote];
      delayValue = 1000000 / melody[currentNote] / 2;
      numCycles = melody[currentNote] * noteDuration / 1000;
      currentCycle = 0;

      digitalWrite(activeBuzzerPin, HIGH);
      lastUpdateTime = millis();
    } else {
      currentState = PLAYING;

      digitalWrite(activeBuzzerPin, HIGH);
      lastUpdateTime = millis();
    }
  }
}

void stopPlayingSong() {
  Serial.println("Knock answered");
  songPlaying = false;

  currentNote = 0;
  currentState = PAUSE;
  noteDuration = 0;
  delayValue = 0;
  numCycles = 0;
  currentCycle = 0;

  digitalWrite(activeBuzzerPin, LOW);
  lastUpdateTime = 0;
}
