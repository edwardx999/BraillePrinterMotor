
class Stepper {
    int _dirpin;
    int _steppin;
  public:
    static constexpr int FORWARDS = HIGH;
    static constexpr int BACKWARDS = LOW;
    Stepper(int dirpin, int steppin): _dirpin(dirpin), _steppin(steppin) {
      pinMode(_dirpin, OUTPUT); // Step
      pinMode(_steppin, OUTPUT); // Dir
    }

    void step(int number_of_steps)
    {
      if (number_of_steps >= 0)
      {
        digitalWrite(_dirpin, FORWARDS);
      }
      else
      {
        number_of_steps = -number_of_steps;
        digitalWrite(_dirpin, BACKWARDS);
      }
      for (int i = 0; i < number_of_steps; ++i)
      {
        digitalWrite(_steppin, HIGH);
        delay(1);
        digitalWrite(_steppin, LOW);
        delay(1);
      }
    }
};
Stepper stepper(6, 3);
void setup() {
  Serial.begin(9600);
}


int num_steps = 0;
int sign=1;
void loop() {
  if (Serial.available())
  {
    unsigned char code = Serial.read();
    switch (code)
    {
      case '-': sign=-sign;break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        num_steps *= 10;
        num_steps += (code - '0');
        break;
      default:
        stepper.step(sign*num_steps);
        num_steps = 0;
        sign=1;
        break;
    }
  }
}
