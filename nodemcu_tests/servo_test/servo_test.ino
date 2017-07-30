#include <Servo.h>

Servo test_servo;

void setup() {
  // put your setup code here, to run once:
  test_servo.attach(D4);
}

void loop() {
  // put your main code here, to run repeatedly:
  test_servo.write(0);
  delay(1000);
  test_servo.write(90);;
  delay(1000);
}
