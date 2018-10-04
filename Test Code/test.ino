#include <AFMotor.h>
uint32_t const pin_la=A3;

uint32_t const pin_slideright=A1;
uint32_t const pin_slideleft=A4;
bool reading=true;
AF_Stepper stepper(48,1);
void setup() {
  pinMode(pin_la,OUTPUT);
  pinMode(pin_slideright,OUTPUT);
  pinMode(pin_slideleft,OUTPUT);
  stepper.setSpeed(200);
}


void loop() {
  stepper.step(18,FORWARD,SINGLE);
  delay(1000);
  stepper.step(18,BACKWARD,SINGLE);
  delay(1000);
  //digitalWrite(pin_la,HIGH);
  //delay(25);
  //digitalWrite(pin_la,LOW);
  //delay(3000);
  /*digitalWrite(pin_slideright,HIGH);
  delay(25);
  digitalWrite(pin_slideright,LOW);
  delay(1500);
  digitalWrite(pin_slideleft,HIGH);
  delay(25);
  digitalWrite(pin_slideleft,LOW);
  delay(1500);*/
}
