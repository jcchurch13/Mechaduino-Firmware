//Contains the declaration of the state variables for the control loop  

#ifndef __STATE_H__
#define __STATE_H__


//interrupt vars

extern volatile float ei;
extern volatile int U;  //control effort (abs)
extern volatile float r;  //setpoint
extern volatile float y;  // measured angle
extern volatile float yw;
extern volatile float yw_1;
extern volatile float e;  // e = r-y (error)
extern volatile float p;  // proportional effort
extern volatile float i;  // integral effort

extern volatile float u;  //real control effort (not abs)
extern volatile float u_1;
extern volatile float e_1;
extern volatile float u_2;
extern volatile float e_2;
extern volatile float u_3;
extern volatile float e_3;
extern volatile long counter;

extern volatile long wrap_count;
extern volatile float y_1;

extern int step_state;
extern long angle; //holds processed angle value
extern float anglefloat;
extern int a;  // raw encoder value in closed loop and print_angle routine (should fix the latter to include LUT)
extern volatile long step_count;  //For step/dir interrupt
extern int stepNumber; // step index for cal routine


extern volatile float ITerm;

extern char mode;


extern int dir;
extern int stepNumber;
extern void output(float,int);




extern volatile float ITerm;

extern volatile char mode;


//___________________________________


extern int val1;
extern int val2;

extern bool print_yw;
#endif

