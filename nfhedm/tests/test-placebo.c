
#include <stdio.h>
#include <placebo.h>

int
main()
{
  double x = 1.5;
  double y = 2.5;
  double z = placebo_1(x, y);
  printf("z: %0.3f\n", z);
  return 0;
}
