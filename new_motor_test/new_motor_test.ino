
int min_modular_distance(int a, int b, int mx)
{
  if (a == b)
  {
    return 0;
  }
  if (a < b)
  {
    int forward_dist = b - a;
    int backward_dist = a + mx - b;
    if (forward_dist <= backward_dist)
    {
      return forward_dist;
    }
    return (backward_dist *= -1);
  }
  else
  {
    int forward_dist = b - a;
    int backward_dist = a - b;
    if (forward_dist <= backward_dist)
    {
      return forward_dist;
    }
    return (backward_dist *= -1);
  }
}
int fix_to_range(int a, int mx)
{
  constexpr bool mod_positive = -5 % 2 == 1;
  if (mod_positive)
  {
    return a % mx;
  }
  else {
    if (a < 0)
    {
      return a % mx + mx;
    }
    return a % mx;
  }
}
using position = signed char;
struct disk_positions {
  position left, right;
};
disk_positions const positions[] = {
  {24, 24},    //0
  {0, 30}, //1
  { 18, 30}, //2
  {0, 0}, //3
  {42, 18},    //4
  {42, 36},   //5
  { 6, 0},  //6
  { 6, 18},  //7
  { 6, 36},  //8
  {36, 0},   //9
  {36, 18},   //10
  { 12, 30},  //11
  {24, 30},   //32
  { 12, 0},  //13
  { 12, 18},  //14
  { 12, 36},  //15
  {24, 0},   //16
  {24, 18},   //17
  {24, 36},   //18
  { 18, 0},  //19
  { 18, 18},  //20
  { 12, 42},  //21
  {24, 42},   //22
  {36, 24},   //23
  { 12, 12},  //24
  { 12, 24},  //25
  { 12, 6},  //26
  {24, 24},   //27
  {24, 6},   //28
  { 6, 12},  //29
  { 6, 24},  //30
  {0, 42},   //31
  { 18, 42},  //32
  {0, 18},   //33
  {0, 24},   //34
  {0, 6},   //35
  { 18, 12},  //36
  { 18, 24},  //37
  { 18, 6},  //38
  {36, 12},   //39
  {36, 24},   //40
  {36, 30},   //41
  { 6, 30},  //42
  {36, 36},   //43
  {36, 6},   //44
  {36, 42},   //45
  { 6, 36},  //46 EXCLAMATION POINT
  { 6, 6},  //47
  { 6, 42},  //48
  {42, 36},   //49
  {42, 6},   //50
  {42, 0},   //51
  {42, 12},   //52
  {42, 30},   //53
  {42, 18},   //54
  {42, 30},   //55
  {42, 42},   //530
  {30, 0},   //57
  {30, 18},   //58
  {30, 24},   //59
  {30, 36},   //60
  {30, 12},   //61
  {30, 6},   //62
  {30, 42}   //63 CAPITAL
  //{,},    //64
};
template<int Max>
class Stepper {
  public:
    static constexpr int FORWARDS = HIGH;
    static constexpr int BACKWARDS = LOW;
    static constexpr int SIGNAL_DELAY = 1;
  private:
    int _pos;
    int _dir_pin;
    int _step_pin;
    void step1()
    {
      digitalWrite(_step_pin, HIGH);
      delay(SIGNAL_DELAY);
      digitalWrite(_step_pin, LOW);
      delay(SIGNAL_DELAY);
    }
    void raw_step(int num_steps)
    {
      step1();
      for (int i = 1; i < num_steps; ++i)
      {
        step1();
      }
    }
  public:

    Stepper(int dir, int step, int start_pos = 0): _dir_pin(dir), _step_pin(step), _pos(0) {
      pinMode(_dir_pin, OUTPUT);
      pinMode(_step_pin, OUTPUT);
    }

    void reset_position(int pos = 0) {
      _pos = pos;
    }

    int position() const {
      return _pos;
    }

    void step(int number_of_steps)
    {
      if (number_of_steps == 0)
      {
        return;
      }
      if (number_of_steps >= 0)
      {
        _pos = fix_to_range(_pos + number_of_steps, Max);
        digitalWrite(_dir_pin, FORWARDS);
        raw_step(number_of_steps);
      }
      else
      {
        _pos = fix_to_range(_pos - number_of_steps, Max);
        digitalWrite(_dir_pin, BACKWARDS);
        raw_step(-number_of_steps);
      }
    }

    void step_to(int pos) {
      pos = fix_to_range(pos, Max);
      int diff = min_modular_distance(_pos, pos, Max);
      if (diff == 0)
      {
        return;
      }
      if (diff > 0)
      {
        _pos = pos;
        digitalWrite(_dir_pin, FORWARDS);
        raw_step(diff);
      }
      else {
        digitalWrite(_dir_pin, BACKWARDS);
        raw_step(-diff);
      }
    }
    friend void multistep(Stepper* steppers, int* steps, size_t num)
    {
      for (size_t i = 0; i < num; ++i)
      {
        auto& stepper = steppers[i];
        auto& step = steps[i];
        if (step == 0) continue;
        if (step > 0)
        {
          stepper._pos = fix_to_range(stepper._pos + step, Max);
          digitalWrite(stepper._dir_pin, FORWARDS);
        }
        else {
          stepper._pos = fix_to_range(stepper._pos - step, Max);
          digitalWrite(stepper._dir_pin, BACKWARDS);
          step = -step;
        }
      }
      while (true) {
        bool finished=true;
        for (size_t i = 0; i < num; ++i)
        {
            auto& stepper = steppers[i];
            auto& step = steps[i];
            if(step>0)
            {
              digitalWrite(stepper._step_pin,HIGH);
              --step;
              finished=false;
            }
        }
        delay(SIGNAL_DELAY);
        for (size_t i = 0; i < num; ++i)
        {
            auto& stepper = steppers[i];
            digitalWrite(stepper._step_pin,LOW);
        }
        if(finished)
        {
          break;
        }
      }
    }
};

Stepper<48> steppers[2]={{6,3},{5,4}};
auto& stepper=steppers[0];
auto& stepper2=steppers[1];
int const pneumatic_port = 22;
void setup() {
  Serial.begin(9600);
  pinMode(pneumatic_port, OUTPUT);
  digitalWrite(pneumatic_port, 0);
}


int num_steps = 0;
int sign = 1;
bool pneumatic = 0;
void loop() {
  //stepper.step(5);
  if (Serial.available())
  {
    unsigned char code = Serial.read();
    switch (code)
    {
      case 'p':
        {
          pneumatic = !pneumatic;
          digitalWrite(pneumatic_port, pneumatic);
          break;
        }
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
      case '-':
        {
          auto const count = sign * num_steps;
          int steps[]={count,count};
          multistep(steppers,steps,2);
          num_steps = 0;
          sign = -1;
          break;
        }
      default:
        {
          auto const count = sign * num_steps;
          int steps[]={count,count};
          multistep(steppers,steps,2);
          num_steps = 0;
          sign = 1;
          break;
        }
    }
  }
}
