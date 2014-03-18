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

#include "checks.h"

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
    double LatticeConstant;
    double Wavelength;
    int nRings;
    double ExcludePoleAngle;
    long long int SizeObsSpots;
    double XGrain[3];
    double YGrain[3];
    double OmegaStart;
    double OmegaStep;
    double px;
    double gs;
    int NoOfOmegaRanges;
    int NrPixelsGrid;
    int RingNumbers[MAX_N_OMEGA_RANGES];
    double OmegaRanges[MAX_N_OMEGA_RANGES][2];
    double BoxSizes[MAX_N_OMEGA_RANGES][4];
    double **P0;
    int *ObsSpotsInfo;
};

double problem_function(
    unsigned n,
	const double *x,
	double *grad,
	void* f_data_trial)
{
	struct my_func_data *f_data = (struct my_func_data *) f_data_trial;
	int i, j;
	const int NrOfFiles = f_data->NrOfFiles;
    const int nLayers = f_data->nLayers;
    const double LatticeConstant = f_data->LatticeConstant;
    const double Wavelength = f_data->Wavelength;
    const int nRings = f_data->nRings;
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
    int RingNumbers[MAX_N_OMEGA_RANGES];
    double OmegaRanges[MAX_N_OMEGA_RANGES][2];
    double BoxSizes[MAX_N_OMEGA_RANGES][4];
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
		RingNumbers[i] = f_data->RingNumbers[i];
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
    CalcOverlapAccOrient(NrOfFiles,nLayers,LatticeConstant,Wavelength,nRings,ExcludePoleAngle,Lsd,SizeObsSpots,XGrain,YGrain,
		RotMatTilts,OmegaStart,OmegaStep,px,ybc,zbc,gs,RingNumbers,OmegaRanges,NoOfOmegaRanges,BoxSizes,P0,NrPixelsGrid,
		ObsSpotsInfo,OrientMatIn,&FracOverlap);
    return (1 - FracOverlap);
}

void
FitOrientation(
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
    double TiltsOrig[3],
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
    int ObsSpotsInfo[SizeObsSpots],
    double EulerIn[3],
    double tol,
    double *EulerOutA,
    double *EulerOutB,
    double *EulerOutC,
    double *ResultFracOverlap,
    double *LsdFit,
    double *TiltsFit,
    double **BCsFit,
    double tolLsd,
    double tolLsdRel,
    double tolTilts,
    double tolBCs)
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
        xl[i] = x[i] - tolBCs;
        xl[i+nLayers] = x[i+nLayers] - tolBCs;
        xu[i] = x[i] + tolBCs;
        xu[i+nLayers] = x[i+nLayers] + tolBCs;
    }
	struct my_func_data f_data;
	f_data.NrOfFiles = NrOfFiles;
	f_data.nLayers = nLayers;
	f_data.LatticeConstant = LatticeConstant;
	f_data.Wavelength = Wavelength;
	f_data.nRings = nRings;
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
		f_data.RingNumbers[i] = RingNumbers[i];
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
	opt = nlopt_create(NLOPT_LN_NELDERMEAD, n);
	nlopt_set_lower_bounds(opt, xl);
	nlopt_set_upper_bounds(opt, xu);
	nlopt_set_min_objective(opt, problem_function, trp);
	double minf;
	nlopt_optimize(opt, x, &minf);
	nlopt_destroy(opt);
	// if (minf < 0.7){
	// 	printf("\n\nA good point found, trying global optimization.\nBefore global optimization: %f\n",1-minf);
	// 	nlopt_opt local_opt;
	// 	double stopval = 0;
	// 	opt = nlopt_create(NLOPT_G_MLSL_LDS, n);
	// 	local_opt = nlopt_create(NLOPT_LN_NELDERMEAD, n);
	// 	nlopt_set_lower_bounds(opt, xl);
	// 	nlopt_set_upper_bounds(opt, xu);
	// 	nlopt_set_min_objective(opt, problem_function, trp);
	// 	nlopt_set_stopval(opt, stopval);
	// 	nlopt_set_ftol_abs(opt, tole);
	// 	nlopt_set_xtol_rel(opt, tole);
	// 	nlopt_set_local_optimizer(opt, local_opt);
	// 	nlopt_optimize(opt, x, &minf);
	// 	nlopt_destroy(local_opt);
	// 	nlopt_destroy(opt);
	// 	printf("After global optimization: %f\n",1-minf);
	// 	opt = nlopt_create(NLOPT_LN_NELDERMEAD, n);
	// 	nlopt_set_lower_bounds(opt, xl);
	// 	nlopt_set_upper_bounds(opt, xu);
	// 	nlopt_set_min_objective(opt, problem_function, trp);
	// 	nlopt_optimize(opt, x, &minf);
	// 	nlopt_destroy(opt);
	// 	printf("After local optimization: %f (Final result!)\n",1-minf);
	// }
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
                    double tol, double lsdtol, double lsdtolrel,
                    double tiltstol,double bctol, double *output, int outputMax)
{
  double EulerIn[3],OrientIn[3][3], FracOut, EulerOutA, EulerOutB,EulerOutC,BestFrac,XG[3],YG[3],OMTemp[9];
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
  FitOrientation(nrFiles,nLayers,LatticeConstant,Wavelength,nRings,ExcludePoleAngle,Lsd,SizeObsSpots,
                 XG,YG,TiltsOrig,OmegaStart,OmegaStep,px,ybc,zbc,gs,RingNumbers,OmegaRanges,NoOfOmegaRanges,
                 BoxSizes,P0,NrPixelsGrid,ObsSpotsInfo,EulerIn,tol,&EulerOutA,&EulerOutB,&EulerOutC,&FracOut,
                 LsdFit,
                 TiltsFit,
                 BCsFit,
                 lsdtol,
                 lsdtolrel,
                 tiltstol,
                 bctol);
  if ((1-FracOut)>BestFrac){
    BestFrac = 1-FracOut;
    for (int j=0;j<9;j++) {
      printf("%f ",OrientMatrixRow[j]);
    }
    printf("\nEuler angles: %f %f %f, ConfidenceIndex: %f, Before fit: %f\nLsd: %f %f %f, Tilts: %f %f %f\nBCs:\t%f %f\n\t%f %f\n\t%f %f\n\n",
           EulerOutA, EulerOutB, EulerOutC, 1-FracOut, OrientMatrixRow[9], LsdFit[0], LsdFit[1], LsdFit[2], TiltsFit[0], TiltsFit[1],
           TiltsFit[2], BCsFit[0][0], BCsFit[0][1], BCsFit[1][0], BCsFit[1][1], BCsFit[2][0], BCsFit[2][1]);
  }
  output[0] = EulerOutA;
  output[1] = EulerOutB;
  output[2] = EulerOutC;
}
