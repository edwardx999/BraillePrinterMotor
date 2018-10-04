#include <AFMotor.h>
AF_Stepper test_motor1(48, 1);
AF_Stepper test_motor2(48, 2);

void setup() {
  Serial.begin(9600);
  test_motor1.setSpeed(200);
  test_motor1.step(18, FORWARD, SINGLE);
  delay(1000);
  test_motor2.setSpeed(200);
  test_motor2.step(18, FORWARD, SINGLE);
  pinMode(A5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    Serial.read();
    digitalWrite(A5, HIGH);
    digitalWrite(13, HIGH);
    digitalWrite(9, LOW);                //feed paper in command ---->
    digitalWrite(2, HIGH);
    delay(250);
    digitalWrite(9, LOW);                //feed paper in command ---->
    digitalWrite(2, LOW);
  }
}
