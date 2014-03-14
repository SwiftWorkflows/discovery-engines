
#include <stdio.h>

#include "placebo.h"

double
placebo_1(double x, double y)
{
  double z = x + y;
  printf("%0.2f = %0.2f + %0.2f\n", z, x, y);
  return z;
}
