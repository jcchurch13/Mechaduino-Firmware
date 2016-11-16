//Contains TC5 Controller definition
//The main control loop is executed by the TC5 timer interrupt:


#include <SPI.h>

#include "State.h" 
#include "Utils.h"
#include "Parameters.h"


void TC5_Handler(){   // gets called with FPID frequency

  
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
    hybridControl();
  }
  else{
      switch (mode) {
  
        case 'x':
          positionControl();  
          break;
          
        case 'v':
          velocityControl();
          break;
  
        case 't':
          torqueControl();
          break;
  
        default:
          u = 0;
          break;
      }

  
  //update phase excitation angle
      if (u > 0) {
        y+=PA;
      }
      else {
        y -=PA;
      }
  
  
  
 // limit control effort 
      if (u > uMAX) {                          //saturation limits max current command
        u = uMAX;
      }
      else if (u < -uMAX) {
        u = -uMAX;
      }
  
      U = abs(u);  
  
 // turn on LED if error is less than 0.1 
      if (abs(e) < 0.1) {
        digitalWrite(ledPin, HIGH);
       //   SerialUSB.println(r);
      }
      else  {
        digitalWrite(ledPin, LOW);
      }


  // update phase currents
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
