//Contains the definitions of the functions used by the firmware.


#include <SPI.h>
#include <Wire.h>

#include "Parameters.h"
#include "Controller.h"
#include "Utils.h"
#include "State.h"


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

void setupSPI() {

  SPISettings settingsA(400000, MSBFIRST, SPI_MODE1);             ///400000, MSBFIRST, SPI_MODE1);

  SPI.begin();    //AS5047D SPI uses mode=1 (CPOL=0, CPHA=1)
  SerialUSB.println("Begin...");
  delay(1000);
  SPI.beginTransaction(settingsA);

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

void commandW() {

  int encoderReading = 0;     //or float?  not sure if we can average for more res?
  int lastencoderReading = 0;
  int avg = 10;         //how many readings to average

  int iStart = 0;
  int jStart = 0;
  int stepNo = 0;

  int fullStepReadings[spr];
  int fullStep = 0;
  //  float newLookup[cpr];
  int ticks = 0;

  float lookupAngle = 0.0;

  encoderReading = readEncoder();
  dir = 1;
  oneStep();
  delay(500);

  if ((readEncoder() - encoderReading) < 0)
  {
    //dir = 0;
    SerialUSB.println("Wired backwards");
    return;
  }

  while (stepNumber != 0) {
    if (stepNumber > 0) {
      dir = 1;
    }
    else
    {
      dir = 0;
    }
    oneStep();
    delay(100);
  }
  dir = 1;
  for (int x = 0; x < spr; x++) {

    encoderReading = 0;
    delay(100);

    for (int reading = 0; reading < avg; reading++) {
      encoderReading += readEncoder();
      delay(10);
    }

    encoderReading = encoderReading / avg;

    anglefloat = encoderReading * 0.02197265625;
    fullStepReadings[x] = encoderReading;
    SerialUSB.println(fullStepReadings[x], DEC);
    oneStep();
  }
  SerialUSB.println(" ");
  SerialUSB.println("ticks:");
  SerialUSB.println(" ");
  for (int i = 0; i < spr; i++) {
    ticks = fullStepReadings[mod((i + 1), spr)] - fullStepReadings[mod((i), spr)];
    if (ticks < -15000) {
      ticks += cpr;

    }
    else if (ticks > 15000) {
      ticks -= cpr;
    }
    SerialUSB.println(ticks);

    if (ticks > 1) {
      for (int j = 0; j < ticks; j++) {
        stepNo = (mod(fullStepReadings[i] + j, cpr));
        // SerialUSB.println(stepNo);
        if (stepNo == 0) {
          iStart = i;
          jStart = j;
        }

      }
    }

    if (ticks < 1) {
      for (int j = -ticks; j > 0; j--) {
        stepNo = (mod(fullStepReadings[199 - i] + j, cpr));
        // SerialUSB.println(stepNo);
        if (stepNo == 0) {
          iStart = i;
          jStart = j;
        }

      }
    }



  }




  SerialUSB.println(" ");
  SerialUSB.println("newLookup:");
  SerialUSB.println(" ");

  for (int i = iStart; i < (iStart + spr); i++) {
    ticks = fullStepReadings[mod((i + 1), spr)] - fullStepReadings[mod((i), spr)];

    if (ticks < -15000) {
      ticks += cpr;

    }
    else if (ticks > 15000) {
      ticks -= cpr;
    }
    //SerialUSB.println(ticks);

    if (ticks > 1) {
      for (int j = jStart; j < (jStart + ticks); j++) {
        lookupAngle = 0.01 * mod(100 * (aps * i + (aps * j / ticks)), 36000.0);
        SerialUSB.print(lookupAngle);
        SerialUSB.print(" , ");
      }
    }
    else if (ticks < 1) {
      for (int j = jStart - ticks; j > (jStart); j--) {
        lookupAngle = 0.01 * mod(100 * (aps * (i) + (aps * (ticks + j) / ticks)), 36000.0);
        SerialUSB.print(lookupAngle);
        SerialUSB.print(" , ");
      }
    }

  }
  SerialUSB.println(" ");


}


void serialCheck() {

  if (SerialUSB.available()) {

    char inChar = (char)SerialUSB.read();

    switch (inChar) {


      case 'p':             //print
        print_angle();
        break;

      case 's':             //step
        oneStep();
        print_angle();
        break;

      case 'd':             //dir
        if (dir == 1) {
          dir = 0;
        }
        else {
          dir = 1;
        }
        break;

      case 'w':
        commandW();           //cal routine
        break;

      case 'e':
        readEncoderDiagnostics();   //encoder error?
        break;

      case 'y':
        enableTCInterrupts();      //enable closed loop
        break;

      case 'n':
        disableTCInterrupts();      //disable closed loop
        break;

      case 'r':             //new setpoint
        SerialUSB.println("Enter setpoint:");
        while (SerialUSB.available() == 0)  {}
        r = SerialUSB.parseFloat();
        break;

      case 'x':
        mode = 'x';           //position loop
        break;

      case 'v':
        mode = 'v';           //velocity loop
        break;

      case 't':
        mode = 't';           //torque loop
        break;

      case 'c':
        mode = 'c';           //custom loop
        break;

      case 'q':
        parameterQuery();     // prints copy-able parameters
        break;

      case 'a':             //anticogging
        antiCoggingCal();
        break;

      case 'k':
        { 
          parameterEditmain();
          
          break;
        }

      default:
        break;
    }
  }

}


void parameterQuery() {
  SerialUSB.println(' ');
  SerialUSB.println("----Current Parameters-----");
  SerialUSB.println(' ');
  SerialUSB.println(' ');

  SerialUSB.print("volatile float Ts = ");
  SerialUSB.print(Ts, DEC);
  SerialUSB.println(";");
  SerialUSB.println(' ');

  SerialUSB.print("volatile float pKp = ");
  SerialUSB.print(pKp);
  SerialUSB.println(";");

  SerialUSB.print("volatile float pKi = ");
  SerialUSB.print(pKi);
  SerialUSB.println(";");

  SerialUSB.print("volatile float pKd = ");
  SerialUSB.print(pKd);
  SerialUSB.println(";");

  SerialUSB.println(' ');

  SerialUSB.print("cvolatile float vKp = ");
  SerialUSB.print(vKp);
  SerialUSB.println(";");

  SerialUSB.print("volatile float vKi = ");
  SerialUSB.print(vKi / Ts);
  SerialUSB.println(" * Ts;");

  SerialUSB.print("volatile float vKd = ");
  SerialUSB.print(vKd * Ts);
  SerialUSB.println(" / Ts;");

  SerialUSB.println(' ');

  SerialUSB.println("const PROGMEM float lookup[] = {");
  for (int i = 0; i < 16384; i++) {
    SerialUSB.print(lookup_angle(i));
    SerialUSB.print(", ");
  }
  SerialUSB.println("");
  SerialUSB.println("};");



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

void readEncoderDiagnostics()           //////////////////////////////////////////////////////   READENCODERDIAGNOSTICS   ////////////////////////////
{
  long angleTemp;
  digitalWrite(chipSelectPin, LOW);



  ///////////////////////////////////////////////READ DIAAGC (0x3FFC)
  SerialUSB.print("DIAAGC (0x3FFC)   ");

  SPI.transfer(0xFF);
  SPI.transfer(0xFC);
  digitalWrite(chipSelectPin, HIGH);

  delay(1);
  digitalWrite(chipSelectPin, LOW);

  byte b1 = SPI.transfer(0xC0);
  byte b2 = SPI.transfer(0x00);


  angleTemp = (((b1 << 8) | b2) & 0B1111111111111111);
  SerialUSB.print((angleTemp | 0B1110000000000000000 ), BIN);

  if (angleTemp & (1 << 14)) {
    SerialUSB.print("  Error occurred  ");
  }
  if (angleTemp & (1 << 11)) {
    SerialUSB.print("  MAGH  ");
  }
  if (angleTemp & (1 << 10)) {
    SerialUSB.print("  MAGL  ");
  }
  if (angleTemp & (1 << 9)) {
    SerialUSB.print("  COF  ");
  }
  if (angleTemp & (1 << 8)) {
    SerialUSB.print("  LF  ");
  }
  SerialUSB.println(" ");

  digitalWrite(chipSelectPin, HIGH);


  delay(1);

  digitalWrite(chipSelectPin, LOW);
  ///////////////////////////////////////////////READ ERRFL (0x0001)
  SerialUSB.print("ERRFL (0x0001)   ");

  SPI.transfer(0x40);
  SPI.transfer(0x01);
  digitalWrite(chipSelectPin, HIGH);

  delay(1);
  digitalWrite(chipSelectPin, LOW);

  b1 = SPI.transfer(0xC0);
  b2 = SPI.transfer(0x00);


  angleTemp = (((b1 << 8) | b2) & 0B1111111111111111);
  SerialUSB.print((angleTemp | 0B1110000000000000000 ), BIN);

  if (angleTemp & (1 << 14)) {
    SerialUSB.print("  Error occurred  ");
  }
  if (angleTemp & (1 << 2)) {
    SerialUSB.print("  parity error ");
  }
  if (angleTemp & (1 << 1)) {
    SerialUSB.print("  invalid register  ");
  }
  if (angleTemp & (1 << 0)) {
    SerialUSB.print("  framing error  ");
  }

  SerialUSB.println(" ");

  digitalWrite(chipSelectPin, HIGH);


  delay(1);

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

int mod(int xMod, int mMod) {
  return (xMod % mMod + mMod) % mMod;
}


float lookup_force(int m)        /////////////////////////////////////////////////  LOOKUP_force   /////////////////////////////
{
  float b_out;
  //
  //  m = (0.01*(((m % 62832) + 62832) % 62832))+0.5;  //+0.5 for rounding
  //
  //  //SerialUSB.println(m);
  //
  //  if (m > 314) {
  //    m = m - 314;
  //    b_out = -pgm_read_float_near(force_lookup + m);
  //
  //  }
  //  else
  //  {
  b_out = pgm_read_float_near(force_lookup + m);
  //  }

  return b_out;
}

float lookup_sine(int m)        /////////////////////////////////////////////////  LOOKUP_SINE   /////////////////////////////
{
  float b_out;

  m = (0.01 * (((m % 62832) + 62832) % 62832)) + 0.5; //+0.5 for rounding

  //SerialUSB.println(m);

  if (m > 314) {
    m = m - 314;
    b_out = -pgm_read_float_near(sine_lookup + m);

  }
  else
  {
    b_out = pgm_read_float_near(sine_lookup + m);
  }

  return b_out;
}


void setupTCInterrupts() {



  // Enable GCLK for TC4 and TC5 (timer counter input clock)
  GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5));
  while (GCLK->STATUS.bit.SYNCBUSY);

  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;   // Disable TCx
  WAIT_TC16_REGS_SYNC(TC5)                      // wait for sync

  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;   // Set Timer counter Mode to 16 bits
  WAIT_TC16_REGS_SYNC(TC5)

  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ; // Set TC as normal Normal Frq
  WAIT_TC16_REGS_SYNC(TC5)

  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1;   // Set perscaler
  WAIT_TC16_REGS_SYNC(TC5)


  TC5->COUNT16.CC[0].reg = 0x3E72; //0x4AF0;
  WAIT_TC16_REGS_SYNC(TC5)


  TC5->COUNT16.INTENSET.reg = 0;              // disable all interrupts
  TC5->COUNT16.INTENSET.bit.OVF = 1;          // enable overfollow
  TC5->COUNT16.INTENSET.bit.MC0 = 1;         // enable compare match to CC0


  NVIC_SetPriority(TC5_IRQn, 1);

  
  // Enable InterruptVector
  NVIC_EnableIRQ(TC5_IRQn);


  // Enable TC
  //  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  //  WAIT_TC16_REGS_SYNC(TC5)




}

void enableTCInterrupts() {

  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;    //Enable TC5
  WAIT_TC16_REGS_SYNC(TC5)                      //wait for sync
}

void disableTCInterrupts() {


  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;   // Disable TC5
  WAIT_TC16_REGS_SYNC(TC5)                      // wait for sync
}


void antiCoggingCal() {
  SerialUSB.println(" -----------------BEGIN ANTICOGGING CALIBRATION!----------------");
  mode = 'x';
  r = lookup_angle(1);
  enableTCInterrupts();
  delay(1000);


  for (int i = 1; i < 657; i++) {
    r = lookup_angle(i);
    SerialUSB.print(r, DEC);
    SerialUSB.print(" , ");
    delay(100);
    SerialUSB.println(u, DEC);
  }
  SerialUSB.println(" -----------------REVERSE!----------------");

  for (int i = 656; i > 0; i--) {
    r = lookup_angle(i);
    SerialUSB.print(r, DEC);
    SerialUSB.print(" , ");
    delay(100);
    SerialUSB.println(u, DEC);
  }
  SerialUSB.println(" -----------------DONE!----------------");
  disableTCInterrupts();
}
 
 
void parameterEditmain() {

    SerialUSB.println();
    SerialUSB.println("Edit parameters:");
    SerialUSB.println();
    SerialUSB.println("p ----- proportional loop");
    SerialUSB.println("v ----- velocity loop");
    SerialUSB.println("o ----- other");
    SerialUSB.println("q ----- quit");
    SerialUSB.println();

    while (SerialUSB.available() == 0)  {}
    char inChar2 = (char)SerialUSB.read();

    switch (inChar2) {
      case 'p':
      {
         parameterEditp();
      }
        break;

      case 'v':
      {    
       parameterEditv();     
      }      
        break;

      case 'o':
      {
        parameterEdito();
      }
        break;
      default:
      {}
        break;
    
   

          }
}

void parameterEditp(){


  SerialUSB.println("Edit position loop gains:");
        SerialUSB.println();
        SerialUSB.print("p ----- pKp = ");
        SerialUSB.println(pKp,DEC);
        SerialUSB.print("i ----- pKi = ");
        SerialUSB.println(pKi,DEC);
        SerialUSB.print("d ----- pKd = ");
        SerialUSB.println(pKd,DEC);
        SerialUSB.println("q ----- quit");
        SerialUSB.println();
        
        while (SerialUSB.available() == 0)  {}
        char inChar3 = (char)SerialUSB.read();
        
        switch (inChar3) {
            case 'p':
              {
              SerialUSB.println("pKp = ?");
              while (SerialUSB.available() == 0)  {}
              pKp = SerialUSB.parseFloat();
              }
              break;
            case 'i':
              {
              SerialUSB.println("pKi = ?");
              while (SerialUSB.available() == 0)  {}
              pKi = SerialUSB.parseFloat();
              }
              break;
            case 'd':  
              {
              SerialUSB.println("pKd = ?");
              while (SerialUSB.available() == 0)  {}
              pKd = SerialUSB.parseFloat();
              }
              break;
            default:
            {}
              break;
        }
}

void parameterEditv(){
  SerialUSB.println("Edit velocity loop gains:");
  SerialUSB.println();
  SerialUSB.print("p ----- vKp = ");
  SerialUSB.println(vKp,DEC);
  SerialUSB.print("i ----- vKi = ");
  SerialUSB.println(vKi,DEC);
  SerialUSB.print("d ----- vKd = ");
  SerialUSB.println(vKd,DEC);
  SerialUSB.println("q ----- quit");
  SerialUSB.println();
  
  while (SerialUSB.available() == 0)  {}
  char inChar4 = (char)SerialUSB.read();
  
  switch (inChar4) {
      case 'p':
        {
        SerialUSB.println("vKp = ?");
        while (SerialUSB.available() == 0)  {}
        vKp = SerialUSB.parseFloat();
        }
        break;
      case 'i':
        {
        SerialUSB.println("vKi = ?");
        while (SerialUSB.available() == 0)  {}
        vKi = SerialUSB.parseFloat();
        }
        break;
      case 'd':  
        {
        SerialUSB.println("vKd = ?");
        while (SerialUSB.available() == 0)  {}
        vKd = SerialUSB.parseFloat();
        }
        break;
      default:
      {}
        break;             
  }
}

void parameterEdito(){


  SerialUSB.println("Edit other parameters:");
        SerialUSB.println();
        SerialUSB.print("p ----- PA = ");
        SerialUSB.println(PA,DEC);
        SerialUSB.println();

        
        while (SerialUSB.available() == 0)  {}
        char inChar3 = (char)SerialUSB.read();
        
        switch (inChar3) {
            case 'p':
              {
              SerialUSB.println("PA = ?");
              while (SerialUSB.available() == 0)  {}
              PA = SerialUSB.parseFloat();
              }
       
              break;
            default:
            {}
              break;
        }
}
