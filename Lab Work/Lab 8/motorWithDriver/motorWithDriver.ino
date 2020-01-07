const int enPin = 9;
const int motorPin1 = 7;
const int motorPin2 = 8;

void setup() {
  // put your setup code here, to run once:
  pinMode(enPin, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  setMotor(255);
}

void setMotor(int speed) {
  analogWrite(enPin, speed);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}
