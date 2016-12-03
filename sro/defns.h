
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
