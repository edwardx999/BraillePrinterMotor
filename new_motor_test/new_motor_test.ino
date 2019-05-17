#include <inostdpolyfill.h>

template<typename Int, typename Int2, typename Int3>
ino::make_signed_t<ino::common_type_t<Int, Int2, Int3>> min_modular_distance(Int from, Int2 to, Int3 mx)
{
  using Ret = ino::make_signed_t<ino::common_type_t<Int, Int2, Int3>>;
  Ret const fixed_a = from;
  Ret const fixed_b = to;
  Ret const fixed_max = mx;
  if (fixed_a == fixed_b)
  {
    return 0;
  }
  if (fixed_a < fixed_b)
  {
    Ret forward_dist = fixed_b - fixed_a;
    Ret backward_dist = fixed_a + fixed_max - fixed_b;
    if (forward_dist <= backward_dist)
    {
      return forward_dist;
    }
    return (backward_dist *= -1);
  }
  else
  {
    Ret forward_dist = fixed_b - fixed_a;
    Ret backward_dist = fixed_a - fixed_b;
    if (forward_dist <= backward_dist)
    {
      return forward_dist;
    }
    return (backward_dist *= -1);
  }
}
template<typename A, typename B>
ino::make_unsigned_t<ino::common_type_t<A, B>> fix_to_range(A a, B mx)
{
  constexpr bool mod_positive = -5 % 2 == 1;
  if (mod_positive)
  {
    return a % mx;
  }
  else
  {
    if (a < 0)
    {
      return a % mx + mx;
    }
    return a % mx;
  }
}

using position = signed char;
class disk_positions {
    position _poses[2];
  public:
    disk_positions(position left, position right): _poses{left, right} {}
    position const* get() const {
      return _poses;
    }
    position* get() {
      return _poses;
    }
    position left() const {
      return _poses[0];
    }
    position right() const {
      return _poses[1];
    }
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

template<typename Device>
class enable_guard {
    Device* _device;
  public:
    enable_guard() = delete;
    enable_guard(enable_guard const&) = delete;
    enable_guard(Device& device): _device{&device} {
      _device->enable();
    }
    ~enable_guard()
    {
      _device->disable();
    }
};

template<typename Device>
class multi_enable_guard {
    ino::span<Device> _devices;
  public:
    multi_enable_guard() = delete;
    multi_enable_guard(multi_enable_guard const&) = delete;
    multi_enable_guard(Device* devices, size_t n): _devices{devices, n} {
      for (auto& device : _devices)
      {
        device.enable();
      }
    }
    template<size_t N>
    multi_enable_guard(Device (&devices)[N]): multi_enable_guard{devices, N} {}
    ~multi_enable_guard()
    {
      for (auto& device : _devices)
      {
        device.disable();
      }
    }
};

template<int Max>
class Stepper {
  public:
    static constexpr auto FORWARDS = HIGH;
    static constexpr auto BACKWARDS = LOW;
    static constexpr int SIGNAL_DELAY = 1;
    static constexpr auto DISABLE = HIGH;
    static constexpr auto ENABLE = LOW;
  private:
    int _pos;
    int _dir_pin;
    int _step_pin;
    int _enable_pin;
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
    Stepper() = delete;
    Stepper(Stepper const&) = delete;
    Stepper(int dir, int step, int enable_pin, int start_pos = 0): _pos{start_pos}, _dir_pin{dir}, _step_pin{step}, _enable_pin{enable_pin}
    {
      pinMode(_dir_pin, OUTPUT);
      pinMode(_step_pin, OUTPUT);
      pinMode(_enable_pin, OUTPUT);
      disable();
    }

    void disable() {
      digitalWrite(_enable_pin, DISABLE);
    }

    void enable() {
      digitalWrite(_enable_pin, ENABLE);
    }

    void reset_position(int pos = 0)
    {
      _pos = fix_to_range(pos, Max);
    }

    int position() const
    {
      return _pos;
    }

    void step(int number_of_steps)
    {
      enable_guard<Stepper> guard{*this};
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

    void step_to(int pos)
    {
      int diff = min_modular_distance(_pos, pos, Max);
      step(diff);
    }
    template<typename Magnitude>
    friend void multistep(Stepper* steppers, Magnitude* steps, size_t num)
    {
      multi_enable_guard<Stepper> guard{steppers, num};
      for (size_t i = 0; i < num; ++i)
      {
        auto& stepper = steppers[i];
        auto& step = steps[i];
        if (step == 0) continue;
        stepper._pos = fix_to_range(stepper._pos + step, Max);
        if (step > 0)
        {
          digitalWrite(stepper._dir_pin, FORWARDS);
        }
        else
        {
          digitalWrite(stepper._dir_pin, BACKWARDS);
          step = -step;
        }
      }
      while (true)
      {
        bool finished = true;
        for (size_t i = 0; i < num; ++i)
        {
          auto& stepper = steppers[i];
          auto& step = steps[i];
          if (step > 0)
          {
            digitalWrite(stepper._step_pin, HIGH);
            --step;
            finished = false;
          }
        }
        delay(SIGNAL_DELAY);
        for (size_t i = 0; i < num; ++i)
        {
          auto& stepper = steppers[i];
          digitalWrite(stepper._step_pin, LOW);
        }
        if (finished)
        {
          break;
        }
        delay(SIGNAL_DELAY);
      }
    }
    template<typename Positions>
    friend void multistep_to(Stepper* steppers, Positions* destinations, size_t count)
    {
      for (size_t i = 0; i < count; ++i)
      {
        destinations[i] = min_modular_distance(steppers[i]._pos, destinations[i], Max);
      }
      multistep(steppers, destinations, count);
    }
};

class PinGuard {
    int _number;
  public:
    PinGuard(int number): _number{number} {
      digitalWrite(_number, HIGH);
    }
    ~PinGuard() {
      digitalWrite(_number, LOW);
    }
};


class MotorForwardGuard;
class MotorBackwardGuard;

class SimpleMotor {
    int _forward_pin;
    int _backward_pin;
    int _enable_pin;
  public:
    SimpleMotor(int forward, int backward, int enable): _forward_pin{forward}, _backward_pin{backward}, _enable_pin{enable} {
      pinMode(forward, OUTPUT);
      pinMode(backward, OUTPUT);
      pinMode(enable, OUTPUT);
    }
    void forward(bool on = true) {
      digitalWrite(_backward_pin, LOW);
      digitalWrite(_forward_pin, on ? HIGH : LOW);
    }
    void backward(bool on = true) {
      digitalWrite(_forward_pin, LOW);
      digitalWrite(_backward_pin, on ? HIGH : LOW);
    }
    void speed(unsigned char spd)
    {
      enable(spd);
    }
    void enable(unsigned char speed = 255) {
      analogWrite(_enable_pin, speed);
    }
    void disable() {
      enable(0);
    }
    friend MotorForwardGuard;
    friend MotorBackwardGuard;
};

namespace detail {
class MotorGuardBase {
    int _pin;
  protected:
    MotorGuardBase(int pin): _pin{pin} {
      digitalWrite(_pin, HIGH);
    }
  public:
    ~MotorGuardBase() {
      digitalWrite(_pin, LOW);
    }
};
}
class MotorForwardGuard: detail::MotorGuardBase {
  public:
    MotorForwardGuard(SimpleMotor& motor): MotorGuardBase{motor._forward_pin} {
    }
};

class MotorBackwardGuard: detail::MotorGuardBase {
    int _pin;
  public:
    MotorBackwardGuard(SimpleMotor& motor): MotorGuardBase{motor._backward_pin} {
    }
};

class Pneumatic {
    int _pin;
  public:
    Pneumatic() = delete;
    Pneumatic(Pneumatic const&) = delete;
    Pneumatic(int pin): _pin{pin} {
      pinMode(_pin, OUTPUT);
    }
    void actuate(bool extend) {
      digitalWrite(_pin, extend ? HIGH : LOW);
    }
    void extend() {
      actuate(true);
    }
    void retract() {
      actuate(false);
    }
};

Stepper<48> steppers[2] = {{6, 7, 8}, {5, 4, 8}};
auto& stepper0 = steppers[0];
auto& stepper1 = steppers[1];
Pneumatic pneumatic{12};
SimpleMotor feed_motor{1, 7, A5};
void setup() {
  Serial.begin(9600);
}

unsigned int pos_index = 0;
bool pos_initialized = false;
bool pneumatic_extended = false;
void loop() {
  if (Serial.available())
  {
    unsigned char in = Serial.read();
    switch (in)
    {
      case 'f':
        {
          Serial.println("Forward Off");
          //analogWrite(A5, 0);
          feed_motor.disable();
          feed_motor.forward(false);
          break;
        }
      case 'F':
        {
          Serial.println("Forward On");
          //analogWrite(A5, 255);
          feed_motor.enable();
          feed_motor.forward();
          break;
        }
      case 'b':
        {
          Serial.println("Backward Off");
          feed_motor.disable();
          feed_motor.backward(false);
          break;
        }
      case 'B':
        {
          Serial.println("Backward On");
          feed_motor.enable();
          feed_motor.backward();
          break;
        }
      case 'p':
      case 'P':
        {
          Serial.println("Pneumatic");
          multi_enable_guard<Stepper<48>> guard{steppers};
          pneumatic_extended = !pneumatic_extended;
          pneumatic.actuate(pneumatic_extended);
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
        pos_index *= 10;
        pos_index += (in - '0');
        pos_initialized = true;
        break;
      default:
        {
          if (pos_initialized)
          {
            Serial.println(pos_index);
            if (pos_index < ino::array_size<decltype(positions)>::value)
            {
              auto dests = positions[pos_index];
              multistep_to(steppers, dests.get(), 2);
            }
            else {
              Serial.println("Invalid Index");
            }
            pos_index = 0;
            pos_initialized = false;
          }
        }
    }
  }
}
