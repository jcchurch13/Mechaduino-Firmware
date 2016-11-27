
/*
  -------------------------------------------------------------
  Mechaduino 0.1 Firmware  v0.1.1
  SAM21D18 (Arduino Zero compatible), AS5047 encoder, A4954 driver

  All Mechaduino related materials are released under the
  Creative Commons Attribution Share-Alike 4.0 License
  https://creativecommons.org/licenses/by-sa/4.0/

  Many thanks to Will Church, Marco Farrugia, and Kai Wolter.
  --------------------------------------------------------------
  
  Controlled via a SerialUSB terminal at 115200 baud.

  Implemented serial commands are:

 s  -  step
 d  -  dir
 p  -  print angle [step count] , [assumed angle] , [encoder reading]

 c  -  calibration routine
 e  -  check encoder diagnositics
 q  -  parameter query

 x  -  position mode
 v  -  velocity mode
 x  -  torque mode

 y  -  enable control loop
 n  -  disable control loop
 r  -  enter new setpoint

 k  -  edit controller gains -- note, these edits are stored in volatile memory and will be reset if power is cycled
 m  -  print main menu

  ...see serialCheck() in Utils for more details

*/


#include "Utils.h"
#include "Parameters.h"
#include "State.h"
#include "analogFastWrite.h"

//////////////////////////////////////
/////////////////SETUP////////////////
//////////////////////////////////////


void setup() {
  digitalWrite(13,HIGH);        //turn LED on 
  setupPins();                  
  setupTCInterrupts();
  sineGen(); 
  
  SerialUSB.begin(115200);
  
  delay(3000);                  //This delay seems to make it easier to establish a connection when the Mechaduino is configured to start in closed loop mode.  
  serialMenu();
  setupSPI();
  
  digitalWrite(13,LOW);         //turn LED off 
  
  pinMode(3, OUTPUT);           //for debugging control loop timing on pin 3

    enableTCInterrupts();     //start in closed loop mode
    mode = 't';

}



//////////////////////////////////////
/////////////////LOOP/////////////////
//////////////////////////////////////

void loop()
{
    
    serialCheck();
    static int val_1 = 0;
    static int val_2 = 0;
    static int val_3 = 0;
    static int val_4 = 0;
    static int val_5 = 0;
    static int val_6 = 0;
    static int val_7 = 0;
    static int val_8 = 0;
    static int val_9 = 0;
    int val = analogRead(0)-512;
    
//    SerialUSB.print(val);
//    SerialUSB.print(" , ");
    //delay(1);
    if (val>10) {
       if (val>30) {
          val = 0;
       }
       else val = 10;

      }
    

    else if (val<-10) {
      if (val<-30) {
          val = 0;
      }
      else val = -10;
    }

    r = 2*(val+val_1+val_2+val_3+val_4+val_5+val_6+val_7+val_8+val_9)+1*((val+val_1+val_2+val_3+val_4+val_5+val_6+val_7+val_8+val_9)/abs((val+val_1+val_2+val_3+val_4+val_5+val_6+val_7+val_8+val_9)));
    SerialUSB.println(r);
    val_9 = val_8;    
    val_8 = val_7;    
    val_7 = val_6;
    val_6 = val_5;
    val_5 = val_4;
    val_4 = val_3;
    val_3 = val_2;
    val_2 = val_1;
    val_1 = val;
  //r=0.1125*step_count; --- no longer need this, step interrupts enabled by default, adjust step angle in parameters.cpp



}
