/*
  MASTER CODE
*/

#include <AFMotor.h>
// Left and right stepper motor
AF_Stepper Lstep(48, 1);    //# steps per rev, port #
AF_Stepper Rstep(48, 2);
#define int unsigned int
int x_max = 35,y_max = 25;
int letter = 0;
int x = 0; // column (i.e. which character)
int y = 0; // row (i.e. which line)
int z = 0; // z = 0 means paper is full or no paper. Consider renaming bool paperFed
// These are being used as digital pins despite being analog
int embossingDonePinA0 = A0; // When DigitalRead() == LOW, embossing is still running
int slideRightCommandPinA1 = A1; // Writing HIGH means sliding the embosser right, LOW means stop (OUTPUT)
int slideRightDonePinA2 = A2; // digitalRead(<-) means sliding carriage over not done (INPUT)
int embossingCommandPinA3 = A3;
int slideLeftHomeCommandPinA4 = A4;
int DCmotorLogic1Pin9 = 9; // These two pins control the actuator. Both are needed to run it
int DCmotorLogic2Pin2 = 2; // However, they can be set in the same or different directions
// /\ 2 pins switch on/off respective sections of the H-bridge (maybe) 
int DCmotorEnablePin13 = 13;
/*
 * FUTURE IMPLEMENTATION:
 * load all letters, convert letters to commands and run through commands
 */

void setup() {
  Serial.begin(9600);
  pinMode(embossingDonePinA0, INPUT);
  pinMode(slideRightDonePinA2, INPUT);
  pinMode(slideRightCommandPinA1, OUTPUT);
  pinMode(embossingCommandPinA3, OUTPUT);
  pinMode(slideLeftHomeCommandPinA4, OUTPUT);
  pinMode(DCmotorLogic1Pin9, OUTPUT);
  pinMode(DCmotorLogic2Pin2, OUTPUT);
  pinMode(DCmotorEnablePin13, OUTPUT);
  //I think we need these two?             *****
  digitalRead(embossingDonePinA0);
  digitalRead(slideRightDonePinA2);
  //digitalWrite all pins initially to LOW, maybe necessary? *****
  Lstep.setSpeed(200);
  Rstep.setSpeed(200);

}

void loop() {
  // These lines don't change any values
  digitalRead(embossingDonePinA0);
  digitalRead(slideRightDonePinA2);

  if (Serial.available() > 0) {
    if (z == 0) {
      digitalWrite(DCmotorEnablePin13, HIGH);
      digitalWrite(DCmotorLogic1Pin9, LOW);                //feed paper in command ---->
      digitalWrite(DCmotorLogic2Pin2, HIGH);
      delay(1000);
      digitalWrite(DCmotorLogic1Pin9, LOW);                //feed paper in command ---->
      digitalWrite(DCmotorLogic2Pin2, LOW);
      z = 1;
    }

    if (x == 35 && y < 25) {
      digitalWrite(slideLeftHomeCommandPinA4, HIGH);            //slide left home command ------>
      delay(25);
      digitalWrite(slideLeftHomeCommandPinA4, LOW);

      digitalWrite(DCmotorEnablePin13, HIGH);
      digitalWrite(DCmotorLogic1Pin9, HIGH);            //scroll one line down command
      digitalWrite(DCmotorLogic2Pin2, LOW);
      delay(500);
      digitalWrite(DCmotorEnablePin13, HIGH);
      digitalWrite(DCmotorLogic1Pin9, LOW);
      digitalWrite(DCmotorLogic2Pin2, LOW);
      x = 0;
      ++y;
    }
    else if (y == 25) {
      digitalWrite(DCmotorLogic1Pin9, HIGH);            //scroll paper out command
      digitalWrite(DCmotorLogic2Pin2, LOW);
      delay(5000);
      digitalWrite(DCmotorLogic1Pin9, LOW);
      digitalWrite(DCmotorLogic2Pin2, LOW);
      y = 0;
      z = 0;
    }

    letter = Serial.read();
    //***********************************************THE REST OF THE CODE IS AN ENORMOUS SWITCH STATEMENT*************************************************
    switch (letter) {
      case 'a':
        if (x < 35 && y < 25) {
          // Rotates the disks to the position for letter 'a'
          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          // Lowers the actuators (embosses the letter) and then raises them back up
          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          
          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }
          /* NEED TO KNOW: MOTOR DELAYS OF STOPING MOTOR IN BLOCK ABOVE 
           *               HOW MOTOR REACTS TO ON AND OFF SIGNALS
           *               WHY delay(25) (only once)?
          */
          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;
        }
        break;

      case 'b':
        if (x < 35 && y < 25) {
          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'c':
        if (x < 35 && y < 25) {
          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish; this comment is wrong
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'd':
        if (x < 35 && y < 25) {
          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'e':
        if (x < 35 && y < 25) {
          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'f':
        if (x < 35 && y < 25) {
          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'g':
        if (x < 35 && y < 25) {
          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'h':
        if (x < 35 && y < 25) {
          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'i':
        if (x < 35 && y < 25) {
          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'j':
        if (x < 35 && y < 25) {
          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'k':
        if (x < 35 && y < 25) {
          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'l':
        if (x < 35 && y < 25) {
          Lstep.step(0, FORWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(0, BACKWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'm':
        if (x < 35 && y < 25) {
          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'n':
        if (x < 35 && y < 25) {
          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'o':
        if (x < 35 && y < 25) {
          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'p':
        if (x < 35 && y < 25) {
          Lstep.step(0, FORWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(0, BACKWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'q':
        if (x < 35 && y < 25) {
          Lstep.step(0, FORWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(0, BACKWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'r':
        if (x < 35 && y < 25) {
          Lstep.step(0, FORWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(0, BACKWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 's':
        if (x < 35 && y < 25) {
          Lstep.step(6, BACKWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, FORWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 't':
        if (x < 35 && y < 25) {
          Lstep.step(6, BACKWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, FORWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'u':
        if (x < 35 && y < 25) {
          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'v':
        if (x < 35 && y < 25) {
          Lstep.step(0, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(0, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'w':
        if (x < 35 && y < 25) {
          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'x':
        if (x < 35 && y < 25) {
          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(12, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(12, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'y':
        if (x < 35 && y < 25) {
          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'z':
        if (x < 35 && y < 25) {
          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(18, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(18, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case ',':
        if (x < 35 && y < 25) {
          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case ';':
        if (x < 35 && y < 25) {
          Lstep.step(6, BACKWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, FORWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case ':':
        if (x < 35 && y < 25) {
          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case '.':
        if (x < 35 && y < 25) {
          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(18, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(18, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case '!':
        if (x < 35 && y < 25) {
          Lstep.step(6, BACKWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, FORWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case '?':
        if (x < 35 && y < 25) {
          Lstep.step(6, BACKWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, FORWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case '|'://'“':
        if (x < 35 && y < 25) {
          Lstep.step(6, BACKWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, FORWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case '>'://'”':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(18, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(18, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case '<'://'’':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case '&':
        if (x < 35 && y < 25) {
          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case '=':
        if (x < 35 && y < 25) {
          Lstep.step(0, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(0, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case '(':
        if (x < 35 && y < 25) {
          Lstep.step(6, BACKWARD, SINGLE);
          Rstep.step(18, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, FORWARD, SINGLE);
            Rstep.step(18, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case ')':
        if (x < 35 && y < 25) {
          Lstep.step(6, BACKWARD, SINGLE);
          Rstep.step(18, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, FORWARD, SINGLE);
            Rstep.step(18, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case '-':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          ++x;

        } break;

      case 'A':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'B':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'C':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);
            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'D':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);


            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'E':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'F':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'G':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'H':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);


            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'I':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'J':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);
            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'K':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'L':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(0, FORWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(0, BACKWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);


            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'M':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'N':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'O':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'P':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(0, FORWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(0, BACKWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'Q':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(0, FORWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(0, BACKWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'R':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(0, FORWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(0, BACKWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'S':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(6, BACKWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, FORWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);


            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'T':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(6, BACKWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, FORWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);


            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'U':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'V':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(0, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(0, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);


            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'W':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'X':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(12, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(12, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'Y':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case 'Z':
        if (x < 35 && y < 25) {
          Lstep.step(6, FORWARD, SINGLE);
          Rstep.step(18, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, BACKWARD, SINGLE);
            Rstep.step(18, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, BACKWARD, SINGLE);
          Rstep.step(18, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, FORWARD, SINGLE);
            Rstep.step(18, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '0':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '1':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '2':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);
            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '3':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);
            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '4':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '5':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, FORWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, BACKWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '6':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '7':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);


            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(6, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(6, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '8':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '9':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);


            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(24, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(24, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '*':
        if (x < 35 && y < 25) {
          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(12, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(12, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '$':
        if (x < 35 && y < 25) {
          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(18, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(18, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case ']':
        if (x < 35 && y < 25) {
          Lstep.step(18, BACKWARD, SINGLE);
          Rstep.step(0, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(18, FORWARD, SINGLE);
            Rstep.step(0, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(6, BACKWARD, SINGLE);
          Rstep.step(18, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(6, FORWARD, SINGLE);
            Rstep.step(18, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;

        } break;

      case '[':
        if (x < 35 && y < 25) {
          Lstep.step(12, FORWARD, SINGLE);
          Rstep.step(12, BACKWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(12, BACKWARD, SINGLE);
            Rstep.step(12, FORWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          Lstep.step(24, FORWARD, SINGLE);
          Rstep.step(6, FORWARD, SINGLE);

          digitalWrite(embossingCommandPinA3, HIGH);            //emboss command  ------>
          delay(25);
          digitalWrite(embossingCommandPinA3, LOW);
          delay(25);

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(embossingDonePinA0) == LOW);

          if (digitalRead(embossingDonePinA0) == HIGH) {                //all done signal <-----------
            Lstep.step(24, BACKWARD, SINGLE);
            Rstep.step(6, BACKWARD, SINGLE);

            digitalWrite(slideRightCommandPinA1, HIGH);          //slide right command ------->
            delay(25);
            digitalWrite(slideRightCommandPinA1, LOW);
          }

          do {
            delay(1);                           // wait for embossing to finish
          } while (digitalRead(slideRightDonePinA2) == LOW);

          x = x + 2;
        }
        break;

    }
  }
}



