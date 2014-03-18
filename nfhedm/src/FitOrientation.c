
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <nlopt.h>

#include "SharedFuncsFit.h"

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
    double *Lsd;
    double RotMatTilts[3][3];
    double *ybc;
    double *zbc;
};

static
double problem_function(
    unsigned n,
        const double *x,
        double *grad,
        void* f_data_trial)
{
        struct my_func_data *f_data = (struct my_func_data *) f_data_trial;
        int i, j = 1;
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
        double *Lsd;
        Lsd = &(f_data->Lsd[0]);
        double *ybc;
        ybc = &(f_data->ybc[0]);
        double *zbc;
        zbc = &(f_data->zbc[0]);
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
        double RotMatTilts[3][3];
        for (i=0;i<3;i++){
                for (j=0;j<3;j++){
                        RotMatTilts[i][j] = f_data->RotMatTilts[i][j];
                }
        }
    double OrientMatIn[3][3], FracOverlap, x2[3];
    x2[0] = x[0]; x2[1] = x[1]; x2[2] = x[2];
    Euler2OrientMat(x2,OrientMatIn);
    CalcOverlapAccOrient(NrOfFiles,nLayers,LatticeConstant,Wavelength,nRings,ExcludePoleAngle,Lsd,SizeObsSpots,XGrain,YGrain,RotMatTilts,OmegaStart,OmegaStep,px,ybc,zbc,gs,RingNumbers,OmegaRanges,NoOfOmegaRanges,BoxSizes,P0,NrPixelsGrid,ObsSpotsInfo,OrientMatIn,&FracOverlap);
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
    double Lsd[nLayers],
    const long long int SizeObsSpots,
    const double XGrain[3],
    const double YGrain[3],
    double RotMatTilts[3][3],
    const double OmegaStart,
    const double OmegaStep,
    const double px,
    double ybc[nLayers],
    double zbc[nLayers],
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
    double *ResultFracOverlap)
{
        unsigned n;
    long int i,j;
    n  = 3;
    double x[n],xl[n],xu[n];
    for( i=0; i<n; i++)
    {
        x[i] = EulerIn[i];
        xl[i] = x[i] - tol;
        xu[i] = x[i] + tol;
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
                //printf("%f %f %f %f\n",XG rain[i],f_data.XGrain[i],YGrain[i],f_data.YGrain[i]);
                for (j=0;j<nLayers;j++){
                        f_data.P0[j][i] = P0[j][i];
                }
                for (j=0;j<3;j++){
                        f_data.RotMatTilts[i][j] = RotMatTilts[i][j];
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
        f_data.Lsd = &Lsd[0];
        f_data.ybc = &ybc[0];
        f_data.zbc = &zbc[0];
        f_data.OmegaStart = OmegaStart;
        f_data.OmegaStep = OmegaStep;
        f_data.px = px;
        f_data.gs = gs;
        f_data.NoOfOmegaRanges = NoOfOmegaRanges;
        f_data.NrPixelsGrid = NrPixelsGrid;
        struct my_func_data *f_datat;
        f_datat = &f_data;
        void* trp = (struct my_func_data *) f_datat;
        // double tole = 1e-3;
        nlopt_opt opt;
        opt = nlopt_create(NLOPT_LN_SBPLX, n);
        nlopt_set_lower_bounds(opt, xl);
        nlopt_set_upper_bounds(opt, xu);
        nlopt_set_min_objective(opt, problem_function, trp);
        double minf;
        nlopt_optimize(opt, x, &minf);
        nlopt_destroy(opt);
    *ResultFracOverlap = minf;
    *EulerOutA = x[0];
    *EulerOutB = x[1];
    *EulerOutC = x[2];
}

int FitOrientationAll(int rown, double gs, double px, double tx, double ty, double tz,
                       int nLayers, double *Lsd, double **XY, int NrOrientations,
                       int **NrSpots, double **OrientationMatrix, double **SpotsMat,
                       int nrFiles, double OmegaStart, double OmegaStep, long long int SizeObsSpots,
                       double *ybc, double *zbc, int *ObsSpotsInfo, double minFracOverlap,
                       double LatticeConstant, int Wavelength, int nRings, double ExcludePoleAngle,
                       int *RingNumbers, double OmegaRanges[MAX_N_OMEGA_RANGES][2], int NoOfOmegaRanges,
                       double BoxSizes[MAX_N_OMEGA_RANGES][4],
                       double tol, int TotalDiffrSpots, double xs, double ys)
{
   // Go through each orientation and compare with observed spots.
    clock_t startthis2;
    startthis2 = clock();
    int NrPixelsGrid=2*(ceil((gs*2)/px))*(ceil((gs*2)/px));
    int NrSpotsThis,StartingRowNr;
    double FracOverT;
    double RotMatTilts[3][3], OrientationMatThis[9], OrientationMatThisUnNorm[9];
    RotationTilts(tx,ty,tz,RotMatTilts);
    double **OrientMatrix;
    OrientMatrix = allocMatrix(MAX_POINTS_GRID_GOOD,10);
    int OrientationGoodID=0;
    double MatIn[3],P0[nLayers][3],P0T[3];
    double OrientMatIn[3][3],XG[3],YG[3];
    double ThrSps[MAX_N_SPOTS][3];
    MatIn[0]=0;
    MatIn[1]=0;
    MatIn[2]=0;
    for (int i=0;i<nLayers;i++){
        MatIn[0] = -Lsd[i];
        MatrixMultF(RotMatTilts,MatIn,P0T);
        for (int j=0;j<3;j++){
            P0[i][j] = P0T[j];
        }
    }
    for (int j=0;j<3;j++){
        XG[j] = XY[j][0];
        YG[j] = XY[j][1];
    }
    for (int i=0;i<NrOrientations;i++){
        NrSpotsThis = NrSpots[i][0];
        StartingRowNr = NrSpots[i][1];
        int m = 0;
        for (m=0;m<9;m++){
            OrientationMatThisUnNorm[m] = OrientationMatrix[i][m];
                        if (OrientationMatThisUnNorm[m] == -0.0){
                                OrientationMatThisUnNorm[m] = 0;
                        }
        }
        m=0;
        NormalizeMat(OrientationMatThisUnNorm,OrientationMatThis);
        for (int j=StartingRowNr;j<(StartingRowNr+NrSpotsThis);j++){
            ThrSps[m][0] = SpotsMat[j][0];
            ThrSps[m][1] = SpotsMat[j][1];
            ThrSps[m][2] = SpotsMat[j][2];
            m++;
        }
        Convert9To3x3(OrientationMatThis,OrientMatIn);
        CalcFracOverlap(nrFiles,nLayers,NrSpotsThis,ThrSps,OmegaStart,OmegaStep,XG,YG,Lsd,SizeObsSpots,RotMatTilts,px,
                        ybc,zbc,
                        gs,P0,NrPixelsGrid,ObsSpotsInfo,OrientMatIn,&FracOverT);
        if (FracOverT >= minFracOverlap){
            for (int j=0;j<9;j++){
                OrientMatrix[OrientationGoodID][j] = OrientationMatThis[j];
            }
            OrientMatrix[OrientationGoodID][9] = FracOverT;
            OrientationGoodID++;
        }
    }
    double BestFrac, BestEuler[3];
    if (OrientationGoodID>0){
        double Fractions, EulerIn[3], OrientIn[3][3], FracOut, EulerOutA, EulerOutB,EulerOutC,OMTemp[9];
        BestFrac = -1;
        for (int i=0;i<OrientationGoodID;i++){
            for (int j=0;j<9;j++){
                OMTemp[j] = OrientMatrix[i][j];
            }
            Convert9To3x3(OMTemp,OrientIn);
            OrientMat2Euler(OrientIn,EulerIn);
            FitOrientation(nrFiles,nLayers,LatticeConstant,Wavelength,nRings,ExcludePoleAngle,Lsd,SizeObsSpots,
                           /*9*/XG,YG,RotMatTilts,OmegaStart,OmegaStep,px,ybc,zbc,gs,
                           /*18*/RingNumbers,OmegaRanges,NoOfOmegaRanges,BoxSizes,
                           /*22*/P0,NrPixelsGrid,ObsSpotsInfo,EulerIn,tol,&EulerOutA,&EulerOutB,&EulerOutC,&FracOut);
            Fractions = 1-FracOut;
            if (Fractions > BestFrac){
                BestFrac = Fractions;
                BestEuler[0] = EulerOutA;
                BestEuler[1] = EulerOutB;
                BestEuler[2] = EulerOutC;
            }
        }
    }else{
                printf("No good ID found.\n");
        }
    // Free memory
    FreeMemMatrix(SpotsMat,TotalDiffrSpots);
    FreeMemMatrixInt(NrSpots,NrOrientations);
    free(ObsSpotsInfo);
    FreeMemMatrix(XY,3);
    FreeMemMatrix(OrientationMatrix,NrOrientations);
    FreeMemMatrix(OrientMatrix,MAX_POINTS_GRID_GOOD);
    clock_t end = clock();
    //diftotal = ((double)(startthis-start))/CLOCKS_PER_SEC;
    //printf("Time elapsed in reading bin files: %f [s]\n",diftotal);
    //diftotal = ((double)(startthis2-startthis))/CLOCKS_PER_SEC;
    //printf("Time elapsed in reading orientations: %f [s]\n",diftotal);
    double diftotal = ((double)(end-startthis2))/CLOCKS_PER_SEC;
    //printf("Time elapsed in comparing diffraction spots: %f [s]\n",diftotal);
    printf("%d %d %f %f %f %f %f %f %f\n",rown, OrientationGoodID, diftotal,
           xs, ys, BestEuler[0],BestEuler[1],BestEuler[2],BestFrac);
    return 1;
}
