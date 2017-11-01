#include <AFMotor.h>
char const END_FILE_CHAR = 3;
uint32_t const SERIAL_NUMBER = 9600;
uint32_t const MOVE_HEAD_PIN; //assign these to something
uint32_t const SET_HEAD_PIN;
uint32_t const FEED_PIN;
#define BUFFER_SIZE 500
char job[BUFFER_SIZE];
uint32_t letter_pos;
uint32_t x;//, y, z;
uint32_t const //num_rows = 24,
num_cols = 25;

void load_job();
void feed_paper();
void spool_paper();
bool is_paper_fed();
void move_head();
void print_head();
void set_head();
void wait_pin(uint32_t);
void load_job();

void setup() {
  Serial.begin(SERIAL_NUMBER);
  x = 0;
  //y = ~0;
  //z = 0;
}

void loop() {
  letter = Serial.read();
  if (END_FILE_CHAR != letter) {
    if (num_cols == x || '\n' == letter) {
      x = 0;
      spool_paper();
    }

    move_head();
    set_head();

    wait_pin(FEED_PIN);
    wait_pin(MOVE_HEAD_PIN);
    wait_pin(SET_HEAD_PIN);
    print_head();
    ++x;
  }
}

inline void load_job() {}
inline void feed_paper() {}
inline void spool_paper() {}
inline bool is_paper_fed() {
  return true;
}
inline void move_head() {}
inline void print_head() {}
inline void set_head() {}
inline void wait_pin(uint32_t pin) {
  while (digitalRead(pin) == LOW);
}
void load_job() {
  letter_pos = ~0;
  while (1) {
    job[++letter_pos] = Serial.read();
    if (job[letter_pos] == END_FILE_CHAR) {
      break;
    }
  }
  letter_pos=~0;
}

