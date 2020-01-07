//const int enPin = 9;
const int motorPin1 = 7;
const int motorPin2 = 8;
const int buttonPin = 2;

int reverse = LOW;
int buttonState = LOW;

void setup() {
  // put your setup code here, to run once:
//  pinMode(enPin, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int newButtonState = !analogRead(buttonPin);
  if (newButtonState != buttonState) {
    buttonState = newButtonState;
    if (buttonState == HIGH) {
      reverse = !reverse;
    }
  }
  
  setMotor(reverse);
}

void setMotor(int direction) {
  digitalWrite(motorPin1, direction);
  digitalWrite(motorPin2, !direction);
}
