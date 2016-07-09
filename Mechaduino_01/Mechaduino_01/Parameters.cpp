//Parameters

#include <math.h>
#include <avr/pgmspace.h>
#include <SPI.h>
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
