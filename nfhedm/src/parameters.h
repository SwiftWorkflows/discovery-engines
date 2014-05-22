/*
 * parameters.h
 *
 *  Created on: Mar 6, 2014
 *      Author: wozniak
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdbool.h>

#include "CalcDiffractionSpots.h"

struct parameters
{
    int StartNr,EndNr;
    int nLayers, NrPixels, SpaceGroup;
    char fn[1000],ext[1024],direct[1024];
    double *Lsd;
    double *ybc;
    double *zbc;
    double OmegaStart,OmegaStep, MaxRingRad;
    double ExcludePoleAngle,LatticeConstant[6],Wavelength,minFracOverlap;
    double tol,lsdtol,lsdtolrel,bctola,bctolb,tiltstol;
    double tx, ty, tz, px;
    int NoOfOmegaRanges;
    double OmegaRanges[MAX_N_OMEGA_RANGES][2], BoxSizes[MAX_N_OMEGA_RANGES][4];
};

/**
   @return True on success, false on failure.
 */
bool ReadParameters(const char *filename, struct parameters *params);

#endif
