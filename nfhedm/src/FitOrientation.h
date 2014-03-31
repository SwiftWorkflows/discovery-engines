/*
 * FitOrientation.h
 *
 *  Created on: Mar 18, 2014
 *      Author: wozniak
 */

#ifndef FITORIENTATION_H
#define FITORIENTATION_H

#include "SharedFuncsFit.h"

/**
   @return 1 on success, 0 on failure
 */
int FitOrientationAll(const char *ParameterFileName, int rown);

int FitOrientation_Calc(int rown, double gs, double px, double tx, double ty, double tz,
                       int nLayers, double *Lsd, double **XY, int NrOrientations,
                       int **NrSpots, double **OrientationMatrix, double **SpotsMat,
                       int nrFiles, double OmegaStart, double OmegaStep, long long int SizeObsSpots,
                       double *ybc, double *zbc, int *ObsSpotsInfo, double minFracOverlap,
                       double LatticeConstant, int Wavelength, int nRings, double ExcludePoleAngle,
                       int *RingNumbers, double OmegaRanges[MAX_N_OMEGA_RANGES][2],
                       int NoOfOmegaRanges, double BoxSizes[MAX_N_OMEGA_RANGES][4],
                       double tol, int TotalDiffrSpots, double xs, double ys);

#endif
