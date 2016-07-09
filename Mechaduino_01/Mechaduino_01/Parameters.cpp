//Contains the Mechaduino parameter defintions


#include <Wire.h>

#include "Parameters.h"


//----Current Parameters-----

volatile float Ts = 0.0003333333;

volatile float pKp = 19.75;
volatile float pKi = 0.50;
volatile float pKd = 0.10;

volatile float vKp = 0.05;
volatile float vKi = 200.00 * Ts;
volatile float vKd = 0.00 / Ts;


//////////////////////////////////////
//////////////////PINS////////////////
//////////////////////////////////////
int IN_4 = 6;//11 - 1;
int IN_3 = 5;//12 - 1;
int VREF_2 = 4;//13 - 1;
int VREF_1 = 9;//3;//8-1;
int IN_2 = 7;//10 - 1;
int IN_1 = 8;//9 - 1;
int pulse = 13;//5;
const int ledPin = 13;//5; //LED connected to digital pin 13
const int chipSelectPin = A2;//5;//6; //output to chip select

int step_pin  = 1;
int dir_pin = 0;//2;

//pins for debugging waveforms:
//int sine_out = 3;				
//int encoder_out = 4;



const int spr = 200; //  200 steps per revolution
const float aps = 360.0 / spr; // angle per step
int cpr = 16384; //counts per rev


int dir = 1;		//initialize stepping mode variables
int step_state = 1;		


long angle = 0; //holds processed angle value

float anglefloat = 0;

int a = 0;  // raw encoder value in closed loop and print_angle routine (should fix the latter to include LUT)


volatile long step_count = 0;  //For step/dir interrupt

volatile int interrupted = 0;

int stepNumber = 0; // step index for cal routine
