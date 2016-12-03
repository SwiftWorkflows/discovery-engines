
#pragma once

#include "defns.h"

/** These have to match the Fortran signatures */
void __sro_MOD_test_c(void);
void* __sro_MOD_make_problem_c(void);
void __sro_MOD_problem_set_c(void* ptr,
                             REAL* a_o1v1, REAL* a_o2v2, REAL* a_o1v2,
                             REAL* l, REAL* m, REAL* n,
                             REAL* h10, REAL* h20, REAL* h30,
                             REAL* h11, REAL* h21, REAL* h31,
                             int* h1n, int* h2n, int* h3n);
void __sro_MOD_free_problem_c(void* ptr);

void problem_set(void* ptr,
                 REAL a_o1v1, REAL a_o2v2, REAL a_o1v2,
                 REAL l, REAL m, REAL n,
                 REAL h10, REAL h20, REAL h30,
                 REAL h11, REAL h21, REAL h31,
                 int h1n, int h2n, int h3n);
