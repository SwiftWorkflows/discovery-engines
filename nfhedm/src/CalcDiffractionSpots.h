/*
 * CalcDiffractionSpots.h
 *
 *  Created on: Mar 5, 2014
 *      Author: wozniak
 */

#ifndef CALCDIFFRACTIONSPOTS_H
#define CALCDIFFRACTIONSPOTS_H

#define MAX_N_OMEGA_RANGES 20

int
CalcDiffractionSpots(double LatticeConstant,
        double Wavelength,
        double Distance,
        int nRings,
        double ExcludePoleAngle,
        const int RingNumbers[MAX_N_OMEGA_RANGES],
        double OmegaRanges[MAX_N_OMEGA_RANGES][2],
        int NoOfOmegaRanges,
        double BoxSizes[MAX_N_OMEGA_RANGES][4],
        int *nTspots,
        double OrientMatr[3][3],
        double **TheorSpots);


#endif