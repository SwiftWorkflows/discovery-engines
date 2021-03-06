
#include <stdio.h>

#include <sro.h>

int
main()
{
  printf("test-c-sro\n");
  __sro_calc_MOD_test_c();

  void* ptr;
  ptr = problem_make();
  printf("ptr: %p\n", ptr);

  #if 0
  REAL a_o1v1, a_o2v2, a_o1v2, l, m, n,
    h10, h20, h30, h11, h21, h31;
  int h1n, h2n, h3n;
  a_o1v1 = a_o2v2 = a_o1v2 = l = m = n =
    h10 = h20 = h30 = h11 = h21 = h31 = 42;
  h1n = h2n = h3n = 100;
  problem_set(&ptr, &a_o1v1, &a_o2v2, &a_o1v2,
                          &l, &m, &n,
                          &h10, &h20, &h30,
                          &h11, &h21, &h31,
                          &h1n, &h2n, &h3n);
  __sro_calc_MOD_problem_free_c(&ptr);
  #endif

  return 0;
}
