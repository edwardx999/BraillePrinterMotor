#include <AFMotor.h>

AF_Stepper slideStepper(200, 2);
AF_Stepper actuateStepper(200, 1);

int const doneEmbossPinA0 = A0;
int const slideIncrementPinA1 = A1;
int const doneSlideIncrementPinA2 = A2;
int const embossPinA3 = A3;
int const slideHomePinA4 = A4;
int const slideLeftHomeDonePin10 = 10;

int signl=A5;

void setup() {
  Serial.begin(9600);
  slideStepper.setSpeed(200);
  actuateStepper.setSpeed(200);
  pinMode(doneEmbossPinA0, OUTPUT);
  pinMode(slideIncrementPinA1, INPUT);
  pinMode(doneSlideIncrementPinA2, OUTPUT);
  pinMode(embossPinA3, INPUT);
  pinMode(slideHomePinA4, INPUT);
  pinMode(slideLeftHomeDonePin10, OUTPUT);
  pinMode(signl, OUTPUT);
  digitalRead(slideIncrementPinA1);
  digitalRead(embossPinA3);
  digitalRead(slideHomePinA4);
}

void loop() {
  uint32_t const d = 730;
  if (digitalRead(embossPinA3)==HIGH) {
    digitalWrite(doneEmbossPinA0, HIGH);
    actuateStepper.step(d, FORWARD, DOUBLE);  //FORWARDS=DOWN
    actuateStepper.step(d, BACKWARD, DOUBLE); //BACKWARDS=UP
    digitalWrite(doneEmbossPinA0, LOW);
    //actuateStepper.release();
  }
  
  if (digitalRead(slideIncrementPinA1)==HIGH) {
    //delay(-1);
    digitalWrite(doneSlideIncrementPinA2, HIGH);
    slideStepper.step(40, FORWARD, DOUBLE);
    //delay(10);
    digitalWrite(doneSlideIncrementPinA2, LOW);
    //slideStepper.release();
    
  }

  if (digitalRead(slideHomePinA4)==HIGH) {
    //digitalWrite(signl,HIGH);
    //delay(10);
    //digitalWrite(signl,LOW);
  
    digitalWrite(slideLeftHomeDonePin10, HIGH);
    slideStepper.step(735, BACKWARD, DOUBLE);
    //delay(10);
    digitalWrite(slideLeftHomeDonePin10, LOW);
    //slideStepper.release();
 
  }
}
