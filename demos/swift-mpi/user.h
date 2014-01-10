
#ifdef SWIG
%{
  #include "user.h"
%}
#endif

double mpi_merge(long long int task_comm_int, char* filename);

double f(double x, double y);
