const int motorPin = 3;

int givenSpeed = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//  if (Serial.available() > 0){
//    givenSpeed = Serial.read();
//    analogWrite(motorPin, givenSpeed);
//  }
  analogWrite(motorPin, 255);
//  delay(1);
}
