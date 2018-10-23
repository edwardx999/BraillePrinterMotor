#include <AFMotor.h>
char const END_FILE_CHAR = 3;
uint32_t const SERIAL_NUMBER = 9600;
uint32_t const MOVE_HEAD_PIN; //assign these to something
uint32_t const SET_HEAD_PIN;
uint32_t const EMBOSS_PUSH_PIN = A3;
uint32_t const EMBOSS_DONE_PIN = A0;
uint32_t const FEED_PIN = 9;
uint32_t const FEED_PIN2 = 2;
uint32_t const FEED_READ_PIN = 13;
//#define BUFFER_SIZE 5000
//char job[BUFFER_SIZE];
typedef signed char posn;
typedef struct {
  posn left, right;
} step_posns;
step_posns const posns[65] = { //arduino doesn't support constexpr that well so it's all declared here now
  {0,0},     //0
  {24, 6}, //1
  { -6, 6}, //2
  {24, 24}, //3
  {18, -6},    //4
  {18, 12},   //5
  { -18, 24},  //6
  { -18, -6},  //7
  { -18, 12},  //8
  {12, 24},   //9
  {12, -6},   //10
  { -12, 6},  //11
  {0, 6},   //12
  { -12, 24},  //13
  { -12, -6},  //14
  { -12, 12},  //15
  {0, 24},   //16
  {0, -6},   //17
  {0, 12},   //18
  { -6, 24},  //19
  { -6, -6},  //20
  { -12, 18},  //21
  {0, 18},   //22
  {12, 0},   //23
  { -12, -12},  //24
  { -12, 0},  //25
  { -12, -18},  //26
  {0, 0},   //27
  {0, -18},   //28
  { -18, -12},  //29
  { -18, 0},  //30
  {24, 18},   //31
  { -6, 18},  //32
  {24, -6},   //33
  {24, 0},   //34
  {24, -18},   //35
  { -6, -12},  //36
  { -6, 0},  //37
  { -6, -18},  //38
  {12, -12},   //39
  {12, 0},   //40
  {12, 6},   //41
  { -18, 6},  //42
  {12, 12},   //43
  {12, -18},   //44
  {12, 18},   //45
  { -18, 12},  //46 EXCLAMATION POINT
  { -18, -18},  //47
  { -18, 18},  //48
  {18, 12},   //49
  {18, -18},   //50
  {18, 24},   //51
  {18, -12},   //52
  {18, 6},   //53
  {18, -6},   //54
  {18, 6},   //55
  {18, 18},   //56
  {6, 24},   //57
  {6, -6},   //58
  {6, 0},   //59
  {6, 12},   //60
  {6, -12},   //61
  {6, -18},   //62
  {6, 18}   //63 CAPITAL
  //{,},    //64
};
uint32_t x;//, y, z;
uint32_t const //num_rows = 24,
num_cols = 25;
AF_Stepper step_left(48, 1);
AF_Stepper step_right(48, 2);
step_posns current={0,0};

void motor_signed_step(AF_Stepper& motor, posn pos);
//void load_job();
void feed_paper();
void spool_paper();
bool is_paper_fed();
void move_head();
void print_head();
void set_head(unsigned char code);
void wait_pin(uint32_t);
void load_job();

void setup() {
  step_left.setSpeed(200);
  step_right.setSpeed(200);
  Serial.begin(SERIAL_NUMBER);
  pinMode(EMBOSS_DONE_PIN, INPUT);
  pinMode(EMBOSS_PUSH_PIN, OUTPUT);
  x = 0;
}

void loop() {
  if (Serial.available()) {
    unsigned char code = Serial.read();
    move_head();
    //wait_pin(FEED_PIN);
    //wait_pin(MOVE_HEAD_PIN);
    if (code < 65) {
      set_head(code);
      //wait_pin(SET_HEAD_PIN);
      print_head();
    }
    ++x;
    if(x==num_cols){
      x=0;
      feed_paper();
    }
  }
  else {
    x = 0;
    set_head(0);
  }
}

inline void load_job() {}
inline void feed_paper() {}
inline void spool_paper() {}
inline bool is_paper_fed() {
  return true;
}
inline void move_head() {}
inline void print_head() {
  digitalWrite(EMBOSS_PUSH_PIN, HIGH);
  delay(10);//wait for slave to write EMBOSS_PUSH_PIN to HIGH
  while (digitalRead(EMBOSS_DONE_PIN) == LOW);
  //delay(1000);
  digitalWrite(EMBOSS_PUSH_PIN, LOW);
}
inline void set_head(unsigned char code) {
  step_posns posn = posns[code];
  int difference =  posn.left - current.left;
  motor_signed_step(step_left, difference);
  current.left = posn.left;

  difference = posn.right - current.right;
  motor_signed_step(step_right, difference);
  current.right = posn.right;
}
inline void wait_pin(uint32_t pin) {
  while (digitalRead(pin) == LOW);
}

inline void motor_signed_step(AF_Stepper& motor, posn pos) {
  if (pos < 0) {
    motor.step(-pos, BACKWARD, DOUBLE);
  }
  else if(pos) {
    motor.step(pos, FORWARD, DOUBLE);
  }
}
