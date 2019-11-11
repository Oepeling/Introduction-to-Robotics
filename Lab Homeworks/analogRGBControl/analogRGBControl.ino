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

const RGBType ledPin(11, 10, 9);
const RGBType potPin(A0, A1, A2);

const RGBType potRead(const RGBType& potPin) {
  return RGBType(
    analogRead(potPin.red()),
    analogRead(potPin.green()),
    analogRead(potPin.blue()));
}

const RGBType mapValue(const RGBType& value,
            const int fromLeft, const int fromRight,
            const int toLeft, const int toRight) {
  return RGBType(
    map(value.red(), fromLeft, fromRight, toLeft, toRight),
    map(value.green(), fromLeft, fromRight, toLeft, toRight),
    map(value.blue(), fromLeft, fromRight, toLeft, toRight));
}

void setMode(const RGBType& pin, const int type) {
  pinMode(pin.red(), type);
  pinMode(pin.green(), type);
  pinMode(pin.blue(), type);
}

void setLedColor(const RGBType& ledPin, const RGBType& value) {
  analogWrite(ledPin.red(), value.red());
  analogWrite(ledPin.green(), value.green());
  analogWrite(ledPin.blue(), value.blue());
}

void setup() {
  setMode(ledPin, OUTPUT);
  setMode(potPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  seLedtColor(ledPin, mapValue(potRead(potPin), 0, 1023, 0, 255));
  delay(10);
}
