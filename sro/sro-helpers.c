
#include "sro.h"

void*
problem_make()
{
  return __sro_MOD_problem_make_c();
}

void
problem_set(void* ptr,
            REAL a_o1v1, REAL a_o2v2, REAL a_o1v2,
            REAL l, REAL m, REAL n,
            REAL h10, REAL h20, REAL h30,
            REAL h11, REAL h21, REAL h31,
            int h1n, int h2n, int h3n)
{
  __sro_MOD_problem_set_c(&ptr, &a_o1v1, &a_o2v2, &a_o1v2,
                          &l, &m, &n,
                          &h10, &h20, &h30,
                          &h11, &h21, &h31,
                          &h1n, &h2n, &h3n);
}

void
problem_free(void* ptr)
{
  __sro_MOD_problem_free_c(&ptr);
}
