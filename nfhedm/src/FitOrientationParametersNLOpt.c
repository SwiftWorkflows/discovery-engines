#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <nlopt.h>
#include <stdint.h>

#include "SharedFuncsFit.h"
#include "CalcDiffractionSpots.h"
#include "FitOrientationParametersNLOpt.h"

#define RealType double
#define float32_t float
#define SetBit(A,k)   (A[(k/32)] |=  (1 << (k%32)))
#define ClearBit(A,k) (A[(k/32)] &= ~(1 << (k%32)))
#define TestBit(A,k)  (A[(k/32)] &   (1 << (k%32)))
#define deg2rad 0.0174532925199433
#define rad2deg 57.2957795130823
#define EPS 1e-5

struct my_func_data{
	int NrOfFiles;
    int nLayers;
    double ExcludePoleAngle;
    long long int SizeObsSpots;
    double XGrain[3];
    double YGrain[3];
    double OmegaStart;
    double OmegaStep;
    double px;
    double gs;
    int hkls[5000][4];
    int n_hkls;
    double Thetas[5000];
    int NoOfOmegaRanges;
    int NrPixelsGrid;
    double OmegaRanges[MAX_N_OMEGA_RANGES][2];
    double BoxSizes[MAX_N_OMEGA_RANGES][4];
    double **P0;
    int *ObsSpotsInfo;
};

static
double problem_function(
    unsigned n,
	const double *x,
	double *grad,
	void* f_data_trial)
{
	struct my_func_data *f_data = (struct my_func_data *) f_data_trial;
	int i, j, count = 1;
	const int NrOfFiles = f_data->NrOfFiles;
    const int nLayers = f_data->nLayers;
    const double ExcludePoleAngle = f_data->ExcludePoleAngle;
    const long long int SizeObsSpots = f_data->SizeObsSpots;
    double XGrain[3];
    double YGrain[3];
    const double OmegaStart = f_data->OmegaStart;
    const double OmegaStep = f_data->OmegaStep;
    const double px = f_data->px;
    const double gs = f_data->gs;
    const int NoOfOmegaRanges = f_data->NoOfOmegaRanges;
    const int NrPixelsGrid = f_data->NrPixelsGrid;
    double P0[nLayers][3];
    double OmegaRanges[MAX_N_OMEGA_RANGES][2];
    double BoxSizes[MAX_N_OMEGA_RANGES][4];
    int hkls[5000][4];
    int n_hkls = f_data->n_hkls;
    double Thetas[5000];
    for (i=0;i<5000;i++){
		hkls[i][0] = f_data->hkls[i][0];
		hkls[i][1] = f_data->hkls[i][1];
		hkls[i][2] = f_data->hkls[i][2];
		hkls[i][3] = f_data->hkls[i][3];
		Thetas[i] = f_data->Thetas[i];
	}
    int *ObsSpotsInfo;
	ObsSpotsInfo = &(f_data->ObsSpotsInfo[0]);
	for (i=0;i<3;i++){
		XGrain[i] = f_data->XGrain[i];
		YGrain[i] = f_data->YGrain[i];
		for (j=0;j<nLayers;j++){
			P0[j][i] = f_data->P0[j][i];
		}
	}
	for (i=0;i<MAX_N_OMEGA_RANGES;i++){
		for (j=0;j<2;j++){
			OmegaRanges[i][j] = f_data->OmegaRanges[i][j];
		}
		for (j=0;j<4;j++){
			BoxSizes[i][j] = f_data->BoxSizes[i][j];
		}
	}
    double OrientMatIn[3][3], FracOverlap, EulIn[3];
    EulIn[0] = x[0];
    EulIn[1] = x[1];
    EulIn[2] = x[2];
    Euler2OrientMat(EulIn,OrientMatIn);
    double Lsd[nLayers], ybc[nLayers], zbc[nLayers],tx,ty,tz,RotMatTilts[3][3];
    tx = x[3]; ty = x[4]; tz = x[5]; RotationTilts(tx,ty,tz,RotMatTilts);
    Lsd[0] = x[6];
    for (i=1;i<nLayers;i++){
        Lsd[i] = Lsd[i-1] + x[6+i];
    }
    for (i=0;i<nLayers;i++){
        ybc[i] = x[6+nLayers+i];
        zbc[i] = x[6+nLayers+nLayers+i];
    }
    CalcOverlapAccOrient(NrOfFiles,nLayers,ExcludePoleAngle,Lsd,SizeObsSpots,XGrain,
		YGrain,RotMatTilts,OmegaStart,OmegaStep,px,ybc,zbc,gs,hkls,n_hkls,
		Thetas,OmegaRanges,NoOfOmegaRanges,BoxSizes,P0,NrPixelsGrid,
		ObsSpotsInfo,OrientMatIn,&FracOverlap);
    return (1 - FracOverlap);
}

void
FitOrientationP(
    const int NrOfFiles,
    const int nLayers,
    const double ExcludePoleAngle,
    double Lsd[nLayers],
    const long long int SizeObsSpots,
    const double XGrain[3],
    const double YGrain[3],
    double TiltsOrig[3],
    const double OmegaStart,
    const double OmegaStep,
    const double px,
    double ybc[nLayers],
    double zbc[nLayers],
    const double gs,
    double OmegaRanges[MAX_N_OMEGA_RANGES][2],
    const int NoOfOmegaRanges,
    double BoxSizes[MAX_N_OMEGA_RANGES][4],
    double P0[nLayers][3],
    const int NrPixelsGrid,
    int ObsSpotsInfo[SizeObsSpots],
    double EulerIn[3],
    double tol,
    double *EulerOutA,
    double *EulerOutB,
    double *EulerOutC,
    double *ResultFracOverlap,
    int hkls[5000][4],
    double Thetas[5000],
    int n_hkls,
    double *LsdFit,
    double *TiltsFit,
    double **BCsFit,
    double tolLsd,
    double tolLsdRel,
    double tolTilts,
    double tolBCsa,
    double tolBCsb)
{
    unsigned n;
    long int i,j;
    n  = 6+(nLayers*3);
    double x[n],xl[n],xu[n];
    for (i=0;i<n;i++){
        x[i] = 0;
        xl[i] = 0;
        xu[i] = 0;
    }
    for( i=0; i<3; i++)
    {
        x[i] = EulerIn[i];
        xl[i] = x[i] - (tol*M_PI/180);
        xu[i] = x[i] + (tol*M_PI/180);
    }
    int count = 0;
    for (i=3;i<6;i++)
    {
        x[i] = TiltsOrig[count];
        xl[i] = x[i] - tolTilts;
        xu[i] = x[i] + tolTilts;
        count++;
    }
    count = 0;
    x[6] = Lsd[0];
    xl[6] = x[6] - tolLsd;
    xu[6] = x[6] + tolLsd;
    count++;
    for (i=7;i<nLayers+6;i++)
    {
        x[i] = Lsd[count] - Lsd[count-1];
        xl[i] = x[i] - tolLsdRel;
        xu[i] = x[i] + tolLsdRel;
        count++;
    }
    count = 0;
    for (i=6+nLayers;i<6+(nLayers*2);i++)
    {
        x[i] = ybc[count];
        x[i+nLayers] = zbc[count];
        xl[i] = x[i] - tolBCsa;
        xl[i+nLayers] = x[i+nLayers] - tolBCsb;
        xu[i] = x[i] + tolBCsa;
        xu[i+nLayers] = x[i+nLayers] + tolBCsb;
        count++;
    }
	struct my_func_data f_data;
	f_data.NrOfFiles = NrOfFiles;
	f_data.nLayers = nLayers;
	f_data.n_hkls = n_hkls;
	for (i=0;i<5000;i++){
		f_data.hkls[i][0] = hkls[i][0];
		f_data.hkls[i][1] = hkls[i][1];
		f_data.hkls[i][2] = hkls[i][2];
		f_data.hkls[i][3] = hkls[i][3];
		f_data.Thetas[i] = Thetas[i];
	}
	f_data.ExcludePoleAngle = ExcludePoleAngle;
	f_data.SizeObsSpots = SizeObsSpots;
	f_data.P0 = allocMatrix(nLayers,3);
	for (i=0;i<3;i++){
		f_data.XGrain[i] = XGrain[i];
		f_data.YGrain[i] = YGrain[i];
		for (j=0;j<nLayers;j++){
			f_data.P0[j][i] = P0[j][i];
		}
	}
	for (i=0;i<MAX_N_OMEGA_RANGES;i++){
		for (j=0;j<2;j++){
			f_data.OmegaRanges[i][j] = OmegaRanges[i][j];
		}
		for (j=0;j<4;j++){
			f_data.BoxSizes[i][j] = BoxSizes[i][j];
		}
	}
	f_data.ObsSpotsInfo = &ObsSpotsInfo[0];
	f_data.OmegaStart = OmegaStart;
	f_data.OmegaStep = OmegaStep;
	f_data.px = px;
	f_data.gs = gs;
	f_data.NoOfOmegaRanges = NoOfOmegaRanges;
	f_data.NrPixelsGrid = NrPixelsGrid;
	struct my_func_data *f_datat;
	f_datat = &f_data;
	void* trp = (struct my_func_data *) f_datat;
	nlopt_opt opt;
	opt = nlopt_create(NLOPT_LN_SBPLX, n);
	nlopt_set_lower_bounds(opt, xl);
	nlopt_set_upper_bounds(opt, xu);
	nlopt_set_min_objective(opt, problem_function, trp);
	double minf=1;
	nlopt_optimize(opt, x, &minf);
	nlopt_destroy(opt);
    *ResultFracOverlap = minf;
    *EulerOutA = x[0];
    *EulerOutB = x[1];
    *EulerOutC = x[2];
    TiltsFit[0] = x[3];
    TiltsFit[1] = x[4];
    TiltsFit[2] = x[5];
    LsdFit[0] = x[6];
    for (i=1;i<nLayers;i++){
        LsdFit[i] = LsdFit[i-1] + x[6+i];
    }
    for (i=0;i<nLayers;i++){
        BCsFit[i][0] = x[6+nLayers+i];
        BCsFit[i][1] = x[6+nLayers+nLayers+i];
    }
}

void
optimizeOrientation(double *OrientMatrixRow,
                    double tx, double ty, double tz, int nLayers,
                    int nrFiles, int *ObsSpotsInfo,
                    double LatticeConstant, double Wavelength, int nRings,
                    double ExcludePoleAngle, double *Lsd, int SizeObsSpots,
                    double OmegaStart, double OmegaStep, double px, double *ybc, double *zbc,
                    double gs, /*18*/int *RingNumbers, /*19*/double OmegaRanges[MAX_N_OMEGA_RANGES][2],
                    int NoOfOmegaRanges,
                    /*21*/double BoxSizes[MAX_N_OMEGA_RANGES][4], double P0[nLayers][3], int NrPixelsGrid,
                    double XG[3], double YG[3],
                    double tol, double lsdtol, double lsdtolrel,
                    double tiltstol,double bctola, double bctolb, double *output, int outputMax)
{
  double EulerIn[3],OrientIn[3][3], FracOut, EulerOutA, EulerOutB,EulerOutC,BestFrac,OMTemp[9];
  double *LsdFit, *TiltsFit, **BCsFit;
  double TiltsOrig[3];
  TiltsOrig[0] = tx;
  TiltsOrig[1] = ty;
  TiltsOrig[2] = tz;
  LsdFit = malloc(nLayers*sizeof(*LsdFit));
  TiltsFit = malloc(nLayers+sizeof(*TiltsFit));
  BCsFit = allocMatrix(nLayers,2);
  BestFrac = -1;
  for (int j=0;j<9;j++){
    OMTemp[j] = OrientMatrixRow[j];
  }
  Convert9To3x3(OMTemp,OrientIn);
  OrientMat2Euler(OrientIn,EulerIn);
  FitOrientationP(nrFiles,nLayers,LatticeConstant,Wavelength,nRings,ExcludePoleAngle,Lsd,SizeObsSpots,
                 XG,YG,TiltsOrig,OmegaStart,OmegaStep,px,ybc,zbc,gs,RingNumbers,OmegaRanges,NoOfOmegaRanges,
                 BoxSizes,P0,NrPixelsGrid,ObsSpotsInfo,EulerIn,tol,&EulerOutA,&EulerOutB,&EulerOutC,&FracOut,
                 LsdFit,TiltsFit,BCsFit,lsdtol,lsdtolrel,tiltstol,bctola,bctolb);
  BestFrac = 1-FracOut;
  // This
  printf("\nBest fraction: %f.\n",BestFrac);
  printf("Euler angles: %f %f %f, ConfidenceIndex: %f\nTilts: %f %f %f\n",
  	EulerOutA, EulerOutB, EulerOutC, 1-FracOut,TiltsFit[0], TiltsFit[1],
	TiltsFit[2]);
  for (int j=0;j<nLayers;j++){
	printf("Layer Nr: %d, Lsd: %f, BCs: %f %f\n", j,LsdFit[j],BCsFit[j][0],BCsFit[j][1]);
  }
  // till here can be commented out if needed.
  output[0] = EulerOutA;
  output[1] = EulerOutB;
  output[2] = EulerOutC;
  output[3] = BestFrac;
  output[4] = TiltsFit[0];
  output[5] = TiltsFit[1];
  output[6] = TiltsFit[2];
  for (int j=0;j<nLayers;j++){
	output[7+j] = LsdFit[j];
	output[7+j+nLayers] = BCsFit[j][0];
	output[7+j+(nLayers*2)] = BCsFit[j][1];
  }
}
