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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// #define HAVE_INLINE
// #define GSL_RANGE_CHECK_OFF
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_integration.h>

#define MAX_POINTS_GRID_GOOD 200000
#define MAX_N_SPOTS 200
#define MAX_N_OMEGA_RANGES 20

#define deg2rad 0.0174532925199433
#define rad2deg 57.2957795130823

typedef double RealType;
#define float32_t float

#define CHECK(condition, msg) \
{ if (!(condition)) { printf("%s\n", msg); \
                      printf("\t At: %s:%i\n", __FILE__, __LINE__); \
                    return false; } }

#define READ(data, size, count, fp)           \
 { int actual = fread(data, size, count, fp); \
   CHECK(actual == count, "short read!");     \
 }

#define FGETS(string, count, fp)           \
    { char* t = fgets(string, count, fp);  \
      CHECK(t != NULL, "fgets failed!");   \
    }

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

/**
   @return True on success, false on failure
 */
bool
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
    const double ExcludePoleAngle,
    const double Lsd[nLayers],
    const long long int SizeObsSpots,
    const double XGrain[3],
    const double YGrain[3],
    // double RotMatTilts[3][3],
    gsl_matrix *RotMatTilts,
    const double OmegaStart,
    const double OmegaStep,
    const double px,
    const double ybc[nLayers],
    const double zbc[nLayers],
    const double gs,
    int hkls[5000][4],
    int n_hkls,
    double Thetas[5000],
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
    // double TheorSpots[MAX_N_SPOTS][3],
    gsl_matrix *TheorSpots,
    double OmegaStart,
    double OmegaStep,
    double XGrain[3],
    double YGrain[3],
    const double Lsds[nLayers],
    const long long int SizeObsSpots,
    //double RotMatTilts[3][3],
    gsl_matrix *RotMatTilts,
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

struct Point2D {
    int x, y;
};

static inline int orient2d(struct Point2D a, struct Point2D b, struct Point2D c)
{
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}

/** Matrix-vector multiply */
static void inline
nfhedm_mvm(const gsl_matrix *A, const gsl_vector *x,
             /*OUT*/ gsl_vector * y)
{
    gsl_blas_dgemv(CblasNoTrans, 1.0, A, x, 0.0, y);
}

void PrintVector(const char* name, const double* v, int count);

void PrintMatrixLinear(const char* name, const double* A, int m, int n);

void PrintMatrix(const char* name, int m, int n, const double A[m][n]);

void gsl_vector_print(const char *name, gsl_vector *v);

void gsl_matrix_print(const char *name, gsl_matrix *A);

/** Text output */
void PrintUint16s(  FILE *fp, uint16_t  *data, int count);
/** Text output */
void PrintUint32s(  FILE *fp, uint32_t  *data, int count);
/** Text output */
void PrintFloat32s( FILE *fp, float32_t *data, int count);
void PrintHeader(   FILE *fp, struct Theader *head);
bool ReadHeader(    FILE *fp, struct Theader *head);
/** Write header: network format */
void WriteHeader_hton(  FILE *fp, struct Theader * head);

/** Convert array via hton */
void hton_Uint16s( uint16_t  *data, int count);
/** Convert array via hton */
void hton_Uint32s( uint32_t  *data, int count);
/** Convert array via hton */
void hton_Float32s(float32_t *data, int count);

/** Convert array via ntoh */
void ntoh_Uint16s( uint16_t  *data, int count);
/** Convert array via ntoh */
void ntoh_Uint32s( uint32_t  *data, int count);
/** Convert array via ntoh */
void ntoh_Float32s(float32_t *data, int count);

static inline int min_integer(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}

/**
   Stick double data in space-separated string (good for Tcl)
 */
void dbls2string(double* data, int count, char* output);

#endif
