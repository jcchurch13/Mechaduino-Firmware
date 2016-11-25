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

extern const float stepangle;

extern const int spr; //  200 steps per revolution
extern const float aps; // angle per step
extern int cpr; //counts per rev

extern int dir;		//initialize stepping mode variables
extern int step_state;


extern long angle; //holds processed angle value

extern float anglefloat;

extern int a;  // raw encoder value in closed loop and print_angle routine (should fix the latter to include LUT)


extern volatile long step_count;  //For step/dir interrupt

extern volatile int interrupted;

extern int stepNumber; // step index for cal routine


extern const PROGMEM float sine_lookup[];
extern const PROGMEM float lookup[];
extern const PROGMEM float force_lookup[];

extern  int sin_1[3600];
//extern  int sin_2[3600];

#endif

