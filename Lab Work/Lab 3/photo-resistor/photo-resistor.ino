const int led_pin = 11;
const int photo_resistor_pin = A0;

int photo_resistor_value = 0;
int led_value = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);
  pinMode(photo_resistor_pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  photo_resistor_value = analogRead(photo_resistor_pin);
  led_value = map(photo_resistor_value, 0, 1023, 0, 255);
  analogWrite(led_pin, led_value);

//  Serial.println(photo_resistor_value);
  delay(10);
}
