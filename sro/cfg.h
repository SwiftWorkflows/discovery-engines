
/* CFG.H
 * Compile-time configuration
 */

#pragma once

#ifdef __GFORTRAN__
#define REAL real
#define REAL_HDF H5T_IEEE_F32LE

/*
#define REAL double precision
#define REAL_HDF H5T_IEEE_F64LE
*/
#else
#define REAL float
#endif

#ifndef FILENAME_MAX
// This is set to 4096 by GNU stdio_lim.h
#define FILENAME_MAX 1024
#endif
