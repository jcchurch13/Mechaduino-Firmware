  //Contains the declaration of the state variables for the control loop  


//interrupt vars

volatile float ei = 0.0;
volatile int U = 0;  //control effort (abs)
volatile float r = 0.0;  //setpoint
volatile float y = 0.0;  // measured angle
volatile float yw = 0.0;
volatile float yw_1 = 0.0;
volatile float e = 0.0;  // e = r-y (error)
volatile float p = 0.0;  // proportional effort
volatile float i = 0.0;  // integral effort


volatile float u = 0.0;  //real control effort (not abs)
volatile float u_1 = 0.0;   //value of u at previous time step, etc...
volatile float e_1 = 0.0;
volatile float u_2 = 0.0;
volatile float e_2 = 0.0;
volatile float u_3 = 0.0;
volatile float e_3 = 0.0;
volatile long counter = 0;

volatile long wrap_count = 0;  //keeps track of how many revolutions the motor has gone though (so you can command angles outside of 0-360)
volatile float y_1 = 0;


int step_state = 1;    
long angle = 0; //holds processed angle value
float anglefloat = 0;
int a = 0;  // raw encoder value in closed loop and print_angle routine (should fix the latter to use LUT)
volatile long step_count = 0;  //For step/dir interrupt
int stepNumber = 0; // step index for cal routine


volatile float ITerm;
volatile char mode;
volatile bool dir = true;  
//___________________________________


int val1 = 0;
int val2 = 0;

bool print_yw = false;      //for step response, under development...

