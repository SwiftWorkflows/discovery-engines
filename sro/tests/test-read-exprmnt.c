
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sro.h"

int
main(int argc, char* argv[])
{
  if (argc != 2)
  {
    printf("requires filename argument!\n");
    exit(EXIT_FAILURE);
  }

  char* filename = argv[1];
  char filename_f[FILENAME_MAX];
  strncpy(filename_f, filename, FILENAME_MAX);
  printf("file: %s\n", filename_f);

  void* p = problem_make();

  problem_set(p,
              0, 0, 0, // REAL a_o1v1, REAL a_o2v2, REAL a_o1v2
              0, 0, 0, // REAL l, REAL m, REAL n,
              0, 0, 0, // REAL h10, REAL h20, REAL h30,
              0, 0, 0, /// REAL h11, REAL h21, REAL h31,
              801, 901, 901); // int h1n, int h2n, int h3n

  __sro_calc_MOD_exprmnt_setup(p, filename_f);
  __sro_calc_MOD_exprmnt_setup(p, filename_f);

  return EXIT_SUCCESS;
}
