class RGBType {
 private:
  int red_;
  int green_;
  int blue_;
  
 public:
  RGBType(int red = 0, int green = 0, int blue = 0) :
    red_(red), green_(green), blue_(blue) {}
  RGBType(const RGBType& other) :
    red_(other.red_), green_(other.green_), blue_(other.blue_) {}

  const int& red() const { return red_; }
  const int& green() const { return green_; }
  const int& blue() const { return blue_; }

  void print() const {
    Serial.print("red: ");
    Serial.print(red_);
    Serial.print(" geen: ");
    Serial.print(green_);
    Serial.print(" blue: ");
    Serial.println(blue_);
  }
};

const RGBType led_pin(9, 10, 11);
const RGBType pot_pin(A2, A1, A0);

void setup() {
  setMode(led_pin, OUTPUT);
  setMode(pot_pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  setColor(mapValue(potRead(), 0, 1023, 0, 255));
  delay(10);
}

const RGBType potRead() {
  return RGBType(
    analogRead(pot_pin.red()),
    analogRead(pot_pin.green()),
    analogRead(pot_pin.blue()));
}

const RGBType mapValue(const RGBType& value,
            const int from_l, const int from_r,
            const int to_l, const int to_r) {
  return RGBType(
    map(value.red(), from_l, from_r, to_l, to_r),
    map(value.green(), from_l, from_r, to_l, to_r),
    map(value.blue(), from_l, from_r, to_l, to_r));
}

void setMode(const RGBType& pin, const int type) {
  pinMode(pin.red(), type);
  pinMode(pin.green(), type);
  pinMode(pin.blue(), type);
}

void setColor(const RGBType& value) {
  analogWrite(led_pin.red(), value.red());
  analogWrite(led_pin.green(), value.green());
  analogWrite(led_pin.blue(), value.blue());
}
