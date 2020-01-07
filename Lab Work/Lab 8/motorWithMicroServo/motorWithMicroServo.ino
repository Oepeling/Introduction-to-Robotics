#include <Servo.h>

const int servoPin = 8;

Servo servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(servoPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  // scan from 0 to 180 degrees
  for(int angle = 0; angle < 180; angle++)  
  {                                  
    servo.write(angle);               
    delay(10);                   
  } 
  // now scan back from 180 to 0 degrees
  for(int angle = 180; angle > 0; angle--)    
  {                                
    servo.write(angle);           
    delay(10);       
  }
}
