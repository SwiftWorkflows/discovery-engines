/*
 * parameters.h
 *
 *  Created on: Mar 6, 2014
 *      Author: wozniak
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "CalcDiffractionSpots.h"

struct parameters
{
    int StartNr,EndNr;
    int nLayers;
    char fn[1000],ext[1024];
    double *Lsd;
    double *ybc;
    double *zbc;
    double OmegaStart,OmegaStep;
    int nRings;
    int RingNumbers[20];
    double ExcludePoleAngle,LatticeConstant,Wavelength,minFracOverlap;
    double tol,lsdtol,lsdtolrel,bctola,bctolb,tiltstol;
    double tx, ty, tz, px;
    int NoOfOmegaRanges;
    double OmegaRanges[MAX_N_OMEGA_RANGES][2], BoxSizes[MAX_N_OMEGA_RANGES][4];
};

/**
   @return 1 on success, 0 on failure.
 */
int parameters_read(const char *filename, struct parameters *params);

#endif
