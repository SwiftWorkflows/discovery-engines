
#include <time.h>
#include <sys/time.h>

/**
   Get the current time as a double to microsecond resolution
*/
__attribute__((unused))
static inline double
time_double()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double result = (double)tv.tv_sec + 0.000001 * (double)tv.tv_usec;
  return result;
}
