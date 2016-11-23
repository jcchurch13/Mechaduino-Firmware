//Contains the declarations for the functions used by the firmware

#ifndef __UTILS_H__
#define __UTIL_H__


	void setupPins();
	
	void setupSPI();
		
	void stepInterrupt();

  void dirInterrupt();

	void output(float theta, int effort);	

	void commandW();	
		
	void serialCheck();

	void parameterQuery();
	
	float lookup_angle(int n);
	
	void oneStep(void);
		
	int readEncoder();
	
	void readEncoderDiagnostics();
		
	void print_angle();
		
	void receiveEvent(int howMany);
	
	float lookup_force(int m);
	
	int mod(int xMod, int mMod);
	
	float lookup_sine(int m);
	
	void setupTCInterrupts();
	
	void enableTCInterrupts();
	
	void disableTCInterrupts();
	
	void antiCoggingCal();
	
	void parameterEditmain();
	
	void parameterEditp();
	
	void parameterEditv();
	
	void parameterEdito();
  
  void hybridStep();

  void hybridControl();

  void positionControl();

  void velocityControl();
  
  void torqueControl();

  void serialMenu();
  
  void sineGen();

#endif

