// Run a A4998 Stepstick from an Arduino UNO.
// Paul Hurley Aug 2015
int x; 
#define BAUD (9600)
int stepPin = 3;
int dirPin = 6;

void setup() 
{
  Serial.begin(BAUD);
  pinMode(stepPin,OUTPUT); // Step
  pinMode(dirPin,OUTPUT); // Dir
  digitalWrite(dirPin,HIGH);
}

void loop() 
{
   // Set Dir high
  //Serial.println("Loop 200 steps (1 rev)");
  for(x = 0; x < 1; x++) // Loop 200 times
  {
    digitalWrite(stepPin,HIGH); // Output high
    delay(5); // Wait
    digitalWrite(stepPin,LOW); // Output low
    delay(5); // Wait
  }
  //Serial.println("Pause");
  delay(200); // pause one second
}
