/*
 * SharedFuncsFit.h
 *
 *  Created on: Mar 18, 2014
 *      Author: wozniak
 */

#ifndef SHAREDFUNCSFIT_H
#define SHAREDFUNCSFIT_H

#define MAX_POINTS_GRID_GOOD 200000

typedef double RealType;

void
RotateAroundZ(
    RealType v1[3],
    RealType alpha,
    RealType v2[3]);

void
Convert9To3x3(double MatIn[9],double MatOut[3][3]);

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

#endif
