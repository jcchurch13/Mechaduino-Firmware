//Utils
#include "Parameters.h"
#include "Utils.h"



float lookup_angle(int n)
{
  float a_out;
  a_out = pgm_read_float_near(lookup + n);
  return a_out;
}

