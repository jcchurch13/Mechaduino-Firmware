//Utils

#include "Parameters.h"
#include "Utils.h"
#include "State.h"
#include "Arduino.h"
#include <Wire.h>


float lookup_angle(int n)
{
  float a_out;
  a_out = pgm_read_float_near(lookup + n);
  return a_out;
}

void oneStep() {           /////////////////////////////////   oneStep    ///////////////////////////////

  if (dir == 0) {
    stepNumber += 1;
  }
  else {
    stepNumber -= 1;
  }
  // step_state = ((((stepNumber) % 4) + 4) % 4); // arduino mod does not wrap for negative....

  //output(1.8 * step_state, 128); //1.8 = 90/50

  output(1.8 * stepNumber, 128); //1.8 = 90/50

  delay(10);
}


void receiveEvent(int howMany)
{
  while (1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    SerialUSB.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  SerialUSB.println(x);         // print the integer
  r = 0.1 * ((float)x);
}