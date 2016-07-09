// multi file
/*


  SAM21D18 (Arduino Zero compatible), AS5047 encoder, A4954 driver

  Controlled via a SerialUSB terminal at 115200 baud.


  ____
    |
  0005|-> LED
  0007|-> pulse
  0009|-> IN_1
  0010|-> IN_2
  0012|-> IN_3
  0011|-> IN_4
    |->                   \
  0008|-> VREF_1             \___A4954
  0013|-> VREF_2            _/
   4|->
  0006|-> chipSelectPin
   2|
    |
  SCK|-> clockPin
  MOSI|-> MISO
  MISO|-> MOSI
  ____|


  Implemented commands are:

  p  -  print [step count] , [assumed angle] , [encoder reading]

  c  -  clear step count & assumed angle

  s  -  step

  d  -  dir toggle

  z  -  seek zero position

  g  -  Go! steps around 400 times

  w  -  Same as go, but stores encoder angles to EEPROM

  r  -  returns EEPROM contents

  a  -  prompts user to enter angle

  y  -  sine sweep


*/


#include "Utils.h"


int analogPin = 1;
int val = 0;

int aout = 0;


//////////////////////////////////////
/////////////////SETUP////////////////
//////////////////////////////////////

void setup() {


  setupPins();
  setupSPI();
  setupTCInterrupts();

  SerialUSB.begin(115200);

  // while (!SerialUSB) {};     //wait for serial

  delay(500);


   // enableTCInterrupts();     //start in closed loop mode
  //  mode = 'x';
  //
  //  Wire.begin(4);                // join i2c bus with address #8
  //  Wire.onReceive(receiveEvent); // register event


  SerialUSB.println("Mechaduino 0.1 begin...");

}



//////////////////////////////////////
/////////////////LOOP/////////////////
//////////////////////////////////////

void loop()
{
  //mode = 'x';


  serialCheck();

  // r=0.1125*step_count;



}
