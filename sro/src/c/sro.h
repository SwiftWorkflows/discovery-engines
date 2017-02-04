
#pragma once

#include "cfg.h"

// FORTRAN

/** These have to match the Fortran signatures */
void __sro_calc_MOD_test_c(void);
void* __sro_calc_MOD_problem_make_c(void);
void __sro_calc_MOD_problem_set_c(void* p,
                                  REAL* a_o1v1, REAL* a_o2v2, REAL* a_o1v2,
                                  REAL* l, REAL* m, REAL* n,
                                  REAL* h10, REAL* h20, REAL* h30,
                                  REAL* h11, REAL* h21, REAL* h31,
                                  int* h1n, int* h2n, int* h3n);
void __sro_calc_MOD_problem_set_lmn_c(void* p,
                                      REAL* l, REAL* m, REAL* n);
void __sro_calc_MOD_problem_free_c(void* p);
REAL __sro_calc_MOD_compute_diff(void* p, char* filename);
// void __sro_calc_MOD_exprmnt_setup(void* p, char* filename);

// PROBLEM

/** This is purely for a nicer name */
void* problem_make(void);

void problem_set(void* p,
                 REAL a_o1v1, REAL a_o2v2, REAL a_o1v2,
                 REAL l, REAL m, REAL n,
                 REAL h10, REAL h20, REAL h30,
                 REAL h11, REAL h21, REAL h31,
                 int h1n, int h2n, int h3n);

void problem_set_lmn(void* p,
                     REAL l, REAL m, REAL n);

void problem_free(void* p);

// COMPUTATION

REAL compute_diff(void* p, char* exprmnt_file);
