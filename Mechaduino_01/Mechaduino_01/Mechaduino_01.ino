
/*
  -------------------------------------------------------------
  Mechaduino 0.1 Firmware  v0.1.3
  SAM21D18 (Arduino Zero compatible), AS5047 encoder, A4954 driver

  All Mechaduino related materials are released under the
  Creative Commons Attribution Share-Alike 4.0 License
  https://creativecommons.org/licenses/by-sa/4.0/

  Many thanks to Will Church, Marco Farrugia, Kai Wolter, Trampas Stern, Mike Anton.
  --------------------------------------------------------------
  
  Controlled via a SerialUSB terminal at 115200 baud.

  Implemented serial commands are:

 s  -  step
 d  -  dir
 p  -  print [step number] , [encoder reading]

 c  -  calibration routine
 e  -  check encoder diagnositics
 q  -  parameter query

 x  -  position mode
 v  -  velocity mode
 x  -  torque mode

 y  -  enable control loop
 n  -  disable control loop
 r  -  enter new setpoint

 j  -  step response
 k  -  edit controller gains -- note, these edits are stored in volatile memory and will be reset if power is cycled
 g  -  generate sine commutation table
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


void setup()        // This code runs once at startup
{                         
   
  digitalWrite(ledPin,HIGH);        // turn LED on 
  setupPins();                      // configure pins
  setupTCInterrupts();              // configure controller interrupt

  SerialUSB.begin(115200);          
  delay(3000);                      // This delay seems to make it easier to establish a connection when the Mechaduino is configured to start in closed loop mode.  
  serialMenu();                     // Prints menu to serial monitor
  setupSPI();                       // Sets up SPI for communicating with encoder
  digitalWrite(ledPin,LOW);         // turn LED off 

  // Uncomment the below lines as needed for your application.
  // Leave commented for initial calibration and tuning.
  
  //    configureStepDir();           // Configures setpoint to be controlled by step/dir interface
  //    configureEnablePin();         // Active low, for use wath RAMPS 1.4 or similar
      enableTCInterrupts();          // uncomment this line to start in closed loop 
      mode = 'x';                   // start in position mode

}
  


//////////////////////////////////////
/////////////////LOOP/////////////////
//////////////////////////////////////


void loop()                 // main loop
{

  serialCheck();              //must have this execute in loop for serial commands to function

  //r=0.1125*step_count;        Don't use this anymore, step interrupts enabled above by "configureStepDir()", adjust step size in parameters.cpp
  
  moveRel(10000.0,10000, 200.0);
  delay(2000);
  //while(1){}
  moveRel(-10000.0,10000,200.0);
  delay(2000);
}








void moveRel(float pos_final,float vel_max, float accel){
  float pos = 0;
  float dpos = 0.45;  // "step size" in degrees, smaller is smoother, but will limit max speed, keep below stepper step angle
  float vel = 0; 
  float vel_1 =0;
  int start = micros(); //for debugging

  float accel_x_dpos = accel*dpos;  // pre calculate
  float dpos_x_1000000 = dpos*1000000.0; // pre calculate

  float pos_remaining = pos_final-pos;
  unsigned long dt =0; 
  unsigned long t = micros();
  unsigned long t_1 = t;

  float r0 = r;  //hold initial setpoint

  // Assume we're decelerating and calculate speed along deceleration profile
  
  while (abs(pos_remaining) >(dpos/2)){  //(may not actually reach exactly so leave some margin
  
    if (pos_remaining > 0)        // clockwise
    vel = sqrt(2.0 * pos_remaining * accel);
    else                      // counter clockwise
    vel = -sqrt(2.0 * -pos_remaining * accel);

    if (vel > vel_1)  // Check if we actually need to accelerate in  clockwise direction
      {

      if (vel_1 == 0)  
        vel = sqrt(2.0 * accel_x_dpos);
      else
        vel = vel_1 + abs(accel_x_dpos/ vel_1);
      if (vel > vel_max)
        vel = vel_max;
      }
    else if (vel < vel_1)
    {
    // Need to accelerate in  counter clockwise direction
    if (vel_1 == 0)
      vel = -sqrt(2.0 * accel_x_dpos);
    else
      vel = vel_1 - abs(accel_x_dpos/ vel_1);
    if (vel < -vel_max)
      vel = -vel_max;
    }
  //  SerialUSB.println(vel);
  
 
  dt = abs(dpos_x_1000000 / vel);
  
    while(t < t_1 + dt) {           //wait calculated dt 
    t = micros();
    }
  
  if (vel > 0)  pos += dpos;        //update setpoint
  else if (vel < 0) pos -= dpos;
  r= r0 + pos;
  
  //SerialUSB.print(micros()-start);
  //SerialUSB.print(" , ");
  
  t_1 = t;  
  vel_1 = vel;
  pos_remaining = pos_final-pos;
  
  }
  r = r0 +pos_final;
  //SerialUSB.print(micros()-start);
  
}

















//void moveRel(float pos_final,float vel_max, float accel){
//  float pos = 0;
//  float dpos = 0.1;
//  float vel = 0; 
//  int start = micros();
//
//  float r0 = r;
//  
//  if (pos_final>0){
//    while (pos<pos_final-dpos){
//      if (pos < pos_final/2){
//        pos += dpos;
//        vel = sqrt(2*accel*pos);
//        if (vel>vel_max){
//          vel = vel_max;
//        }
//        else if (vel == 0){
//          vel = 0.01;
//        }
//        
//      }
//      else { 
//        pos += dpos;
//        vel = sqrt(2*accel*(pos_final-pos));
//        if (vel>vel_max){
//          vel = vel_max;
//        }
//        else if (vel == 0){
//          vel = 0.01;
//        }
//        
//      }
//      SerialUSB.print(micros()-start);
//      SerialUSB.print(" , ");
//      //SerialUSB.println(pos);    
//      delay(1000*dpos/vel);
//      r = r0 + pos;
//    }
//  }
//  else{
//  while (pos>pos_final+dpos){
//      if (pos > pos_final/2){
//        pos -= dpos;
//        vel = sqrt(-2*accel*pos);
//        if (vel>vel_max){
//          vel = vel_max;
//        }
//        else if (vel == 0){
//          vel = 0.01;
//        }
//        
//      }
//      else { 
//        pos -= dpos;
//        vel = sqrt(-2*accel*(pos_final-pos));
//        if (vel>vel_max){
//          vel = vel_max;
//        }
//        else if (vel == 0){
//          vel = 0.01;
//        }
//        
//      }
//      SerialUSB.print(micros()-start);
//      SerialUSB.print(" , ");
//      //SerialUSB.println(pos);    
//      delay(1000*dpos/vel);
//      r = r0 + pos;
//    }
//  }
//
//  
//  r = r0 + pos_final;
//  pos = pos_final;
//  SerialUSB.print(micros()-start);
//  SerialUSB.print(" , ");
//  //SerialUSB.println(pos,DEC); 
//  
//}


