//Contains TC5 Controller definition
//The main control loop is executed by the TC5 timer interrupt:


#include <SPI.h>

#include "State.h" 
#include "Utils.h"
#include "Parameters.h"


void TC5_Handler()
{

  //unedited/old:
  /*  // TcCount16* TC = (TcCount16*) TC3; // get timer struct
    if (TC5->COUNT16.INTFLAG.bit.OVF == 1) {  // A overflow caused the interrupt
        interrupted = 1;

      TC5->COUNT16.INTFLAG.bit.OVF = 1;    // writing a one clears the flag ovf flag
    //  irq_ovf_count++;                 // for debug leds
    }

    // if (TC->INTFLAG.bit.MC0 == 1) {  // A compare to cc0 caused the interrupt
    //  digitalWrite(pin_mc0_led, LOW);  // for debug leds
     // TC->INTFLAG.bit.MC0 = 1;    // writing a one clears the flag ovf flag
    // } */



  ///new
  // TcCount16* TC = (TcCount16*) TC3; // get timer struct
  if (TC5->COUNT16.INTFLAG.bit.OVF == 1) {  // A overflow caused the interrupt




    a = readEncoder();
    y = lookup_angle(a);


    if ((y - y_1) < -180.0) {
      wrap_count += 1;
    }
    else if ((y - y_1) > 180.0) {
      wrap_count -= 1;
    }
    //y_1 = y;  pushed lower

    yw = (y + (360.0 * wrap_count));





  if (mode == 'h'){

  static int missed_steps = 0;
  static float iLevel = 0.6;  //hybrid stepping current level.  In this mode, this current is continuous (unlike closed loop mode). Be very careful raising this value as you risk overheating the A4954 driver!
  static float rSense = 0.15;
  
  if (yw < r-1.8) {
    missed_steps -= 1;
  }
  else if (yw > r+1.8) {
    missed_steps += 1;
  }

  output(0.1125 *(-(r-missed_steps)), (255/3.3)*(iLevel*10*rSense)); 
    
  }
  else{
      switch (mode) {
  
        case 'x':
          e = (r - yw);
  
          ITerm += (pKi * e);
          if (ITerm > 150) ITerm = 150;
          else if (ITerm < -150) ITerm = -150;
  
  
          u = ((pKp * e) + ITerm - (pKd * (yw - yw_1))); //ARDUINO library style
          //u = u+lookup_force(a)-20;
          //   u = u_1 + cA*e + cB*e_1 + cC*e_2;     //ppt linked in octave script
  
          //  u = 20*e;//
  
  
  
          break;
  
        case 'v':
  
  
          e = (r - ((yw - yw_1) * Fs*0.16666667));//error in degrees per rpm (sample frequency in Hz * (60 seconds/min) / (360 degrees/rev) )
  
          ITerm += (vKi * e);
          if (ITerm > 200) ITerm = 200;
          else if (ITerm < -200) ITerm = -200;
  
  
          u = ((vKp * e) + ITerm - (vKd * (yw - yw_1)));//+ lookup_force(a)-20; //ARDUINO library style
  
          break;
  
        case 't':
          u = 1.0 * r ;//+ 1.7*(lookup_force(a)-20);
          break;
  
        default:
          u = 0;
          break;
      }
  

  
  
  
  //
  //    if (u > 0) {
  //      PA = 1.8;
  //    }
  //    else {
  //      PA = -1.8;
  //    }
  //
  //    y += PA;
  
  
  
      if (u > 0) {
        y+=PA;
      }
      else {
        y -=PA;
      }
  
  
  
  
  
      if (u > uMAX) {                          //saturation limits max current command
        u = uMAX;
      }
      else if (u < -uMAX) {
        u = -uMAX;
      }
  
  
  
      U = abs(u);       //+lookup_force((((a-4213)%16384)+16384)%16384)-6); ///p);//+i);
  
  
      if (abs(e) < 0.1) {
        digitalWrite(pulse, HIGH);
       //   SerialUSB.println(r);
      }
      else  {
        digitalWrite(pulse, LOW);
      }
  
      output(-y, U);  //-y
  }
      e_3 = e_2;
      e_2 = e_1;
      e_1 = e;
      u_3 = u_2;
      u_2 = u_1;
      u_1 = u;
      yw_1 = yw;
      y_1 = y;

    TC5->COUNT16.INTFLAG.bit.OVF = 1;    // writing a one clears the flag ovf flag
  }


}
