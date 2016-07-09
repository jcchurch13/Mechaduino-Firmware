//Utils

#include "Parameters.h"
#include "Utils.h"
#include "State.h"
#include "Arduino.h"
#include <Wire.h>


void setupPins() {

  pinMode(VREF_2, OUTPUT);
  pinMode(VREF_1, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(pulse, OUTPUT);
  pinMode(step_pin, INPUT);
  pinMode(dir_pin, INPUT);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  attachInterrupt(1, stepInterrupt, RISING);



  analogWrite(VREF_2, 64);
  analogWrite(VREF_1, 64);

  digitalWrite(IN_4, HIGH);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_2, HIGH);
  digitalWrite(IN_1, LOW);

  pinMode(ledPin, OUTPUT); // visual signal of I/O to chip
  // pinMode(clockPin, OUTPUT); // SCK
  pinMode(chipSelectPin, OUTPUT); // CSn -- has to toggle high and low to signal chip to start data transfer
  //  pinMode(inputPin, INPUT); // SDA



}

void stepInterrupt() {
  if (digitalRead(dir_pin))
  {
    step_count += 1;
  }
  else
  {
    step_count -= 1;
  }


}

void output(float theta, int effort) {                    //////////////////////////////////////////   OUTPUT   ///////////////////
  static int start = 0;
  static int finish = 0;
  static int intangle;
  static float floatangle;
  static int modangle;



  floatangle = (10000 * ( theta * 0.87266 + 2.3562) );//0.7854) );// 2.3562) );       //changed to 2.3 for NEMA23,NEMA17 dual..... opposite below
  //floatangle = (10000 * ( theta * 0.87266 + 0.7854) );

  intangle = (int)floatangle;
  //  modangle = (((intangle % 628) + 628) % 628);
  val1 = effort * lookup_sine(intangle);

  analogWrite(VREF_2, abs(val1));

  if (val1 >= 0)  {
    digitalWrite(IN_4, HIGH);
    //     PORTB |= (B00000001);
    digitalWrite(IN_3, LOW);
    //    PORTB &= ~(B00000010);

  }
  else  {
    digitalWrite(IN_4, LOW);
    //  PORTB &= ~(B00000001);
    digitalWrite(IN_3, HIGH);
    //    PORTB |= (B00000010);

  }





  floatangle = (10000 * (  theta * 0.8726646 + 0.7854) );//2.3562) );//0.7854) );
  //floatangle = (10000 * ( theta * 0.87266 + 2.3562) );

  intangle = (int)floatangle;
  // modangle = (((intangle % 628) + 628) % 628);
  val2 = effort * lookup_sine(intangle);

  analogWrite(VREF_1, abs(val2));

  if (val2 >= 0)  {
    digitalWrite(IN_2, HIGH);
    //     PORTB |= (B00000100);
    digitalWrite(IN_1, LOW);
    //     PORTB &= ~(B00001000);

  }
  else  {
    digitalWrite(IN_2, LOW);
    //   PORTB &= ~(B00000100);
    digitalWrite(IN_1, HIGH);
    //   PORTB |= (B00001000);

  }





}


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

int readEncoder()           //////////////////////////////////////////////////////   READENCODER   ////////////////////////////
{
  long angleTemp;
  digitalWrite(chipSelectPin, LOW);

  //angle = SPI.transfer(0xFF);
  byte b1 = SPI.transfer(0xFF);
  byte b2 = SPI.transfer(0xFF);


  angleTemp = (((b1 << 8) | b2) & 0B0011111111111111);
  //  SerialUSB.println((angle & 0B0011111111111111)*0.02197265625);

  digitalWrite(chipSelectPin, HIGH);
  return angleTemp;




}

void print_angle()                ///////////////////////////////////       PRINT_ANGLE   /////////////////////////////////
{
  a = 0;
  delay(100);
  a += readEncoder();
  delay(10);
  a += readEncoder();
  delay(10);
  a += readEncoder();
  delay(10);
  a += readEncoder();
  delay(10);
  a += readEncoder();
  delay(10);
  a += readEncoder();
  delay(10);
  a += readEncoder();
  delay(10);
  a += readEncoder();
  delay(10);
  a += readEncoder();
  delay(10);
  a += readEncoder();
  delay(10);
  a = a / 10;

  anglefloat = a * 0.02197265625;
  SerialUSB.print(stepNumber, DEC);
  SerialUSB.print(" , ");
  SerialUSB.print(stepNumber * aps, DEC);
  SerialUSB.print(" , ");
  SerialUSB.print(a, DEC);
  SerialUSB.print(" , ");
  SerialUSB.println(anglefloat, DEC);
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