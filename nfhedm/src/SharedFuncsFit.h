/*
 * SharedFuncsFit.h
 *
 *  Created on: Mar 18, 2014
 *      Author: wozniak
 */

#ifndef SHAREDFUNCSFIT_H
#define SHAREDFUNCSFIT_H

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS_GRID_GOOD 200000
#define MAX_N_SPOTS 200
#define MAX_N_OMEGA_RANGES 20

typedef double RealType;
#define float32_t float


struct Theader {
    uint32_t uBlockHeader;
    uint16_t BlockType;
    uint16_t DataFormat;
    uint16_t NumChildren;
    uint16_t NameSize;
    char BlockName[4096];
    uint32_t DataSize;
    uint16_t ChunkNumber;
    uint16_t TotalChunks;
};

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

static inline void
realloc_array(void *p, int oldSize, int newSize)
{
    if (p == NULL)
    {
        p = malloc(newSize);
        assert(p != NULL);
        return;
    }
    if (newSize <= oldSize)
        return;

    void *t = realloc(p, newSize);
    assert(t != NULL);
}

static inline void
realloc_buffers(int nElements, int nElements_previous,
                uint16_t **ys, uint16_t **zs, uint16_t **peakID,
                float32_t **intensity)
{
    if (nElements > nElements_previous) {
        *ys = realloc(*ys, nElements*sizeof(**ys));
        *zs = realloc(*zs, nElements*sizeof(**zs));
        *peakID = realloc(*peakID, nElements*sizeof(**peakID));
        *intensity = realloc(*intensity, nElements*sizeof(**intensity));
    }
}

void PrintUint16s( FILE *fp, uint16_t  *data, int count);
void PrintUint32s( FILE *fp, uint32_t  *data, int count);
void PrintFloat32s(FILE *fp, float32_t *data, int count);
void PrintHeader( FILE *fp, struct Theader *head);
void ReadHeader(  FILE *fp, struct Theader *head);

#endif
