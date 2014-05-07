/*
 * CalcDiffractionSpots.h
 *
 *  Created on: Mar 5, 2014
 *      Author: wozniak
 */

#ifndef CALCDIFFRACTIONSPOTS_H
#define CALCDIFFRACTIONSPOTS_H

#define MAX_N_OMEGA_RANGES 20

/**
   @return 0 on success, 1 on error
 */
int
CalcDiffractionSpots(double Distance,
		double ExcludePoleAngle,
        double OmegaRanges[MAX_N_OMEGA_RANGES][2],
        int NoOfOmegaRanges,
		int hkls[5000][4],
		int n_hkls,
		double Thetas[5000],
        double BoxSizes[MAX_N_OMEGA_RANGES][4],
        int *nTspots,
        double OrientMatr[3][3],
        double **TheorSpots);


#endif
