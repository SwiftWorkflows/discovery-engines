/*
 * SharedFuncsFit.h
 *
 *  Created on: Mar 18, 2014
 *      Author: wozniak
 */

#ifndef SHAREDFUNCSFIT_H
#define SHAREDFUNCSFIT_H

#define MAX_POINTS_GRID_GOOD 200000
#define MAX_N_SPOTS 200
#define MAX_N_OMEGA_RANGES 20

typedef double RealType;

void
RotateAroundZ(
    RealType v1[3],
    RealType alpha,
    RealType v2[3]);

void
Convert9To3x3(double MatIn[9],double MatOut[3][3]);

void Euler2OrientMat(
    double Euler[3],
    double m_out[3][3]);

void OrientMat2Euler(
    double m[3][3],
    double Euler[3]);

double**
allocMatrix(int nrows, int ncols);

int**
allocMatrixInt(int nrows, int ncols);

void
FreeMemMatrix(RealType **mat,int nrows);

void
FreeMemMatrixInt(int **mat,int nrows);

int
ReadBinFiles(
    char FileStem[1000],
    char *ext,
    int StartNr,
    int EndNr,
    int *ObsSpotsMat,
    int nLayers,
    long long int ObsSpotsSize);

void
MatrixMultF(
    RealType m[3][3],
    RealType v[3],
    RealType r[3]);

void
CalcOverlapAccOrient(
    const int NrOfFiles,
    const int nLayers,
    const double LatticeConstant,
    const double Wavelength,
    const int nRings,
    const double ExcludePoleAngle,
    const double Lsd[nLayers],
    const long long int SizeObsSpots,
    const double XGrain[3],
    const double YGrain[3],
    double RotMatTilts[3][3],
    const double OmegaStart,
    const double OmegaStep,
    const double px,
    const double ybc[nLayers],
    const double zbc[nLayers],
    const double gs,
    const int RingNumbers[MAX_N_OMEGA_RANGES],
    double OmegaRanges[MAX_N_OMEGA_RANGES][2],
    const int NoOfOmegaRanges,
    double BoxSizes[MAX_N_OMEGA_RANGES][4],
    double P0[nLayers][3],
    const int NrPixelsGrid,
    const int ObsSpotsInfo[SizeObsSpots],
    double OrientMatIn[3][3],
    double *FracOverlap);

void
CalcFracOverlap(
    const int NrOfFiles,
    const int nLayers,
    const int nTspots,
    double TheorSpots[MAX_N_SPOTS][3],
    double OmegaStart,
    double OmegaStep,
    double XGrain[3],
    double YGrain[3],
    const double Lsds[nLayers],
    const long long int SizeObsSpots,
    double RotMatTilts[3][3],
    const double px,
    const double ybcs[nLayers],
    const double zbcs[nLayers],
    const double gs,
    double P0All[nLayers][3],
    const int NrPixelsGrid,
    const int ObsSpotsInfo[SizeObsSpots],
    double OrientMatIn[3][3],
    double *FracOver);

void
NormalizeMat(
    double OMIn[9],
    double OMOut[9]);

void
RotationTilts(
    double tx,
    double ty,
    double tz,
    double RotMatOut[3][3]);

#endif
