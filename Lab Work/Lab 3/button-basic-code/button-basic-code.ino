const int button_pin = 7;
const int led_pin = 13;

int button_value = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  button_value = digitalRead(button_pin);
  
  // Pull up way
  digitalWrite(led_pin, !button_value);
  // Pull down way
  // digitalWrite(led_pin, button_value);
  
  Serial.println(button_value);
  delay(100);
}
