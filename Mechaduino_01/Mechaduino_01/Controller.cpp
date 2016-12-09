//Contains TC5 Controller definition
//The main control loop is executed by the TC5 timer interrupt:


#include <SPI.h>

#include "State.h"
#include "Utils.h"
#include "Parameters.h"


void TC5_Handler() {  // gets called with FPID frequency
  static int print_counter = 0;

  if (TC5->COUNT16.INTFLAG.bit.OVF == 1) {  // An overflow caused the interrupt
     
     TEST2_HIGH();  //digitalWrite(3, HIGH); //Fast Write to Digital 3 for debugging

  y = lookup[readEncoder()];
  
   
    if ((y - y_1) < -180.0) wrap_count += 1;
    else if ((y - y_1) > 180.0) wrap_count -= 1;

    //y_1 = y;  pushed lower

    yw = (y + (360.0 * wrap_count));


    if (mode == 'h') {
      hybridControl();
    }
    else {
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


    if (u > 0)
      {
      y += PA;          //update phase excitation angle
      if (u > uMAX)     // limit control effort
        u = uMAX;       //saturation limits max current command
      }
    else
      {
      y -= PA;          //update phase excitation angle
      if (u < -uMAX)    // limit control effort
        u = -uMAX;      //saturation limits max current command
      }

      U = abs(u);

      if (abs(e) < 0.1) ledPin_HIGH();   //digitalWrite(ledPin, HIGH);  // turn on LED if error is less than 0.1
      else ledPin_LOW();    //digitalWrite(ledPin, LOW);


      output(-y, round(U));    // update phase currents
    }
   // e_3 = e_2;
   // e_2 = e_1;
   // e_1 = e;
   // u_3 = u_2;
   // u_2 = u_1;
   // u_1 = u;
    yw_1 = yw;
    y_1 = y;
    
    if (print_yw ==  true){       //for step resonse... still under development
      print_counter += 1;
      
      if (print_counter >= 5){
        SerialUSB.println(int(yw*1024));
        print_counter = 0;
      }
    }
    TC5->COUNT16.INTFLAG.bit.OVF = 1;    // writing a one clears the flag ovf flag
    TEST2_LOW();

  }


}

