/****************** Const Variables Declarations ******************/

const int trigPin = 9;
const int echoPin = 10;

/******************** Variables Declarations ********************/

int duration = 0;
int distance = 0;

/****************** Function Declarations **********************/

/*************************** Setup *****************************/

void setup() {
  // put your setup code here, to run once:
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

  Serial.print("Distance: ");
  Serial.println(distance);
  delay(5);
}

/****************** Function Definitions **********************/
