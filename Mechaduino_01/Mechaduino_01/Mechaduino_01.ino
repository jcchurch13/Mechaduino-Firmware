  
/*

  Mechaduino 0.1 Firmware  --multi file branch
    
  SAM21D18 (Arduino Zero compatible), AS5047 encoder, A4954 driver



  
All Mechaduino related materials are released under the

Creative Commons Attribution Share-Alike 4.0 License

https://creativecommons.org/licenses/by-sa/4.0/



  Many thanks to Will Church and Marco Farrugia.


  

  
  
  Controlled via a SerialUSB terminal at 115200 baud.

  Implemented serial commands are:


  s  -  step

  d  -  dir toggle

  w  -  encoder cal routine
  
  y  -  enable controller interrupts

  n  - disable controller interrupts

  r  -  enter new setpoint

  x  - position mode

  v  -  velocity mode

  t  -  torque mode

  q  - parameter query (prints current parameters)

  e  -  reads encoder diagnostic register 

  p  -  print [step count] , [assumed angle] , [encoder reading]

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
  SerialUSB.begin(115200);
  delay(3000);          //This delay seems to make it easier to establish a conncetion when the Mechaduino is configured to start in closed loop mode.
  serialMenu();
  setupPins();
  setupSPI();
  setupTCInterrupts();

  
  //  enableTCInterrupts();     //start in closed loop mode
  //  mode = 'x';

}



//////////////////////////////////////
/////////////////LOOP/////////////////
//////////////////////////////////////

void loop()
{
  
  serialCheck();
  
  //r=0.1125*step_count; --- no longer need this adjust step angle in parameters.cpp

  

}

