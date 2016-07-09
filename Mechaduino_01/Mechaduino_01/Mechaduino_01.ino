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







#include <math.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include <Wire.h>


#include "Utils.h"



int analogPin = 1;
int val = 0;

int aout = 0;





//////////////////////////////////////
/////////////////SETUP/////////////////////
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


  //tp

  //pinMode(10, OUTPUT);
  //pinMode(11, OUTPUT);
  //pinMode(12, OUTPUT);
  SerialUSB.println("Mechaduino 0.1 begin...");

}



//////////////////////////////////////
/////////////////LOOP/////////////////////
//////////////////////////////////////



void loop()
{
  //mode = 'x';


  serialCheck();

  // r=0.1125*step_count;

  

  // electronic gearing with analog
  //  val = 0;
  //  for (int i = 0; i < 10; i++){
  //    val += analogRead(analogPin);    // read the input pin
  //   // delay(1);
  //  }
  //
  //  SerialUSB.println(0.01*((float)val));             // debug value
  //  r = 0.01*((float)val);
  //
  //  aout = 10.0*yw;
  //
  //  if (aout > 1023){
  //    aout = 1023;
  //  }
  //  else if (aout < 0){
  //    aout = 0;
  //  }
  //
  //  analogWriteResolution(10);p
  //  analogWrite(A0,aout);
  //


  //  for (int i = 0; i < 1024; i++){
  //    analogWrite(A0,i);
  //    delay(2);
  //  }



  //  // tp dispenser:
  //  mode = 'x';
  //
  //  digitalWrite(10, LOW);
  //  digitalWrite(11, LOW);
  //  digitalWrite(12, HIGH);
  //
  //  if (u > 70) {
  //    digitalWrite(10, LOW);
  //    digitalWrite(11, HIGH);
  //    digitalWrite(12, LOW);
  //    for (int i = 0; i < 2700; i++) {
  //      r -= 0.2;
  //      delay(1);
  //    }
  //      digitalWrite(10, HIGH);
  //      digitalWrite(11, LOW);
  //      digitalWrite(12, LOW);
  //    delay(750);
  //    r -= 45;
  //    delay(50);
  //    r += 135;
  //    delay(250);
  //    for (int i = 0; i < 1200; i++) {
  //      r -= 0.2;
  //      delay(1);
  //    }
  //  }



  //    mode = 'x';















  //  r = 90;
  //  delay(2000);
  //  r = 0;
  //  delay(500);
  //  r = -90;
  //  delay (1000);

  //    if (u>50){
  //      r = -45;
  //      delay(500);
  //    }
  //    else if (u< -50){
  //      r = 45;
  //      delay(500);
  //    }
  //




}
