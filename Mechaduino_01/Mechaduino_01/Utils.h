//Contains the declarations for the functions used by the firmware

#ifndef __UTILS_H__
#define __UTIL_H__


	void setupPins();                 // initializes pins
	
	void setupSPI();                  //initializes SPI
		
	void stepInterrupt();             //step interrupt handler

  void dirInterrupt();              //dir interrupt handler

	void output(float theta, int effort);	  //calculates phase currents (commutation) and outputs to Vref pins

	void calibrate();	                //calibration routine
		
	void serialCheck();               //checks serial port for commands.  Must include this in loop() for serial interface to work

	void parameterQuery();            //Prints current parameters
	
	float lookup_angle(int n);        //takes encoder reading and returns calibration-corrected angle
	
	void oneStep(void);               //take one step
		
	int readEncoder();                //read raw encoder position
	  
	void readEncoderDiagnostics();    //check encoder diagnostics registers
		
	void print_angle();               //for debigging purposes in open loop mode:  prints [step count] , [assumed angle] , [encoder reading]
	
	void receiveEvent(int howMany);   //for i2c interface...
	
	float lookup_force(int m);        //for anti-cogging... under development
	
	int mod(int xMod, int mMod);      //modulo,  handles negative values properly    
	
	void setupTCInterrupts();         //configures control loop interrupt
	
	void enableTCInterrupts();        //enables control loop interrupt.  Use this to enable "closed-loop" modes
	
	void disableTCInterrupts();       //disables control loop interrupt.  Use this to diable "closed-loop" mode
	
	void antiCoggingCal();            //under development...
	
	void parameterEditmain();         //parameter editing menu
	
	void parameterEditp();            //parameter editing menu
	
	void parameterEditv();            //parameter editing menu
	
	void parameterEdito();            //parameter editing menu

  void hybridControl();             //open loop stepping, but corrects for missed steps.  under development

  void positionControl();           //position control code (PID)

  void velocityControl();           //velocity control code (PID)
  
  void torqueControl();             //current control code 

  void serialMenu();                //main menu
  
  void sineGen();                   //generates sinusoidal commutation table. you can experiment with other commutation profiles 

  void stepResponse();
#endif

