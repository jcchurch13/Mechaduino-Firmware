//Contains the Mechaduino parameter declarations

#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__


//----Current Parameters-----

extern volatile float Ts;
extern volatile float Fs;

extern volatile float pKp;
extern volatile float pKi;
extern volatile float pKd;

extern volatile float vKp;
extern volatile float vKi;
extern volatile float vKd;

extern const PROGMEM float lookup[];


extern const int spr; //  200 steps per revolution
extern const float aps; // angle per step
extern int cpr; //counts per rev
extern const float stepangle;
extern volatile float PA;  //

extern const float iMAX;
extern const float rSense;
extern volatile int uMAX;


extern const float pi;

//////////////////////////////////////
//////////////////PINS////////////////
//////////////////////////////////////
extern const int IN_4;
extern const int IN_3;
extern const int VREF_2;
extern const int VREF_1;
extern const int IN_2;
extern const int IN_1;
extern const int pulse;
extern const int ledPin;
extern const int chipSelectPin;

extern const int step_pin;
extern const int dir_pin;




extern const PROGMEM float force_lookup[];

//extern  int sin_1[3600];
extern  int sin_1[3600];

#endif

