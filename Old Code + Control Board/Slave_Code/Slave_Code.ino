#include <AFMotor.h>

AF_Stepper slideStepper(200, 2);
AF_Stepper actuateStepper(200, 1);

int doneEmbossPinA0 = A0;
int slideIncrementPinA1 = A1;
int doneSlideIncrementPinA2 = A2;
int embossPinA3 = A3;
int slideHomePinA4 = A4;
int slideLeftHomeDonePin10 = 10;

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
  if (digitalRead(embossPinA3)) {
    actuateStepper.step(d, FORWARD, DOUBLE);  //FORWARDS=DOWN
    actuateStepper.step(d, BACKWARD, DOUBLE); //BACKWARDS=UP
    digitalWrite(doneEmbossPinA0, HIGH);
    delay(25);
    digitalWrite(doneEmbossPinA0, LOW);
    //actuateStepper.release();
  }
  
  if (digitalRead(slideIncrementPinA1)) {
    //delay(-1);
    
    //slideStepper.step(40, FORWARD, DOUBLE);
    digitalWrite(doneSlideIncrementPinA2, HIGH);
    delay(10);
    digitalWrite(doneSlideIncrementPinA2, LOW);
    //slideStepper.release();
    
  }

  if (digitalRead(slideHomePinA4)) {
    digitalWrite(signl,HIGH);
    delay(10);
    digitalWrite(signl,LOW);
    /*
    slideStepper.step(735, BACKWARD, DOUBLE);
    digitalWrite(slideLeftHomeDonePin10, HIGH);
    delay(10);
    digitalWrite(slideLeftHomeDonePin10, LOW);
    //slideStepper.release();
    */
  }
}
