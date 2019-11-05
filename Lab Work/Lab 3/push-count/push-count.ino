const int button_pin = 7;
const int led_pin = 13;

int button_prev_value = 0;
int button_value = 0;
int button_push_count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  button_value = !digitalRead(button_pin);
  if (button_value == HIGH && button_prev_value == LOW) {
    button_push_count++;
    Serial.println(button_push_count);
  }
  button_prev_value = button_value;
  
  digitalWrite(led_pin, (button_push_count % 4 == 0));

  delay(10);
}
