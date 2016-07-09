//Utils

#ifndef __UTILS_H__
#define __UTIL_H__

	void setupPins();
		
	void stepInterrupt();

	void output(float theta, int effort);		

	float lookup_angle(int n);
	
	void oneStep(void);
		
	int readEncoder();
		
	void print_angle();
		
	void receiveEvent(int howMany);

#endif
