
import io;
import sys;

// Define external MPI function
@par (float t) mpi_merge(string filename) "user" "0.0" "mpi_merge_tcl";

// Define external C function
(float t) f(float x, float y) "user" "0.0"
[ "set <<t>> [ f <<x>> <<y>> ]" ];

main
{
  string data = argv("data");
  float avg = @par=4 mpi_merge(data);
  printf("Swift avg: %0.1f", avg);
  foreach i in [0:4] {
    float t = itof(i);
    float output = f(avg, t);
    printf("Swift output: %0.1f", output);
  }
}
