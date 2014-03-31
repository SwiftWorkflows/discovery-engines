
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <nlopt.h>

#include "checks.h"
#include "parameters.h"
#include "SharedFuncsFit.h"
#include "FitOrientation.h"

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

int FitOrientationAll(const char *ParamFN, int rown, double result[4])
{
    printf("FitOrientationAll(%s,%i)...\n", ParamFN, rown);
    struct parameters params;
    parameters_read(ParamFN, &params);

       //Read bin files
       char fnG[1000], fn[1000];
       sprintf(fnG,"%s/grid.txt",params.direct);
       char fnDS[1000];
       char fnKey[1000];
       char fnOr[1000];
       sprintf(fnDS,"%s/DiffractionSpots.txt",params.direct);
       sprintf(fnKey,"%s/Key.txt",params.direct);
       sprintf(fnOr,"%s/OrientMat.txt",params.direct);
       sprintf(fn,"%s/%s",params.direct,params.fn);
       int i,nrFiles,nrPixels;
       int *ObsSpotsInfo;
       int ReadCode;
       nrFiles = params.EndNr - params.StartNr + 1;
       nrPixels = params.NrPixels*params.NrPixels;
       long long int SizeObsSpots;
       SizeObsSpots = (params.nLayers);
       SizeObsSpots*=nrPixels;
       SizeObsSpots*=nrFiles;
       SizeObsSpots/=32;
       ObsSpotsInfo = malloc(SizeObsSpots*sizeof(ObsSpotsInfo));
       if (ObsSpotsInfo==NULL){
           printf("Could not allocate ObsSpotsInfo.\n");
           return 0;
       }
       ReadCode = ReadBinFiles(params.fn,params.ext,params.StartNr,params.EndNr,ObsSpotsInfo,
                            params.nLayers,SizeObsSpots);
       if (ReadCode == 0){
           printf("Reading bin files did not go well. Please check.\n");
           return 0;
       }

       FILE *fp;
       fp = fopen(fnG,"r");
       if (fp == NULL)
           file_not_found(fnG);
       char line[1024];
       fgets(line,1000,fp);
       int TotalNrSpots=0;
       sscanf(line,"%d",&TotalNrSpots);
       if (rown > TotalNrSpots){
           printf("Error: Grid point number greater than total number of grid points.\n");
           return 0;
       }
       int counter=0;
       double y1,y2,xs,ys,gs;
       double **XY;
       XY = allocMatrix(3,3);
       while(counter<rown){
           fgets(line,1000,fp);
           counter+=1;
       }
       sscanf(line,"%lf %lf %lf %lf %lf",&y1,&y2,&xs,&ys,&gs);
       fclose(fp);
       if (y1>y2){
           XY[0][0] =xs;
           XY[0][1] =ys - y1;
           XY[1][0] =xs - gs;
           XY[1][1] =ys + y2;
           XY[2][0] =xs + gs;
           XY[2][1] =ys + y2;
       }
       else{
           XY[0][0] =xs;
           XY[0][1] =ys + y2;
           XY[1][0] =xs - gs;
           XY[1][1] =ys - y1;
           XY[2][0] =xs + gs;
           XY[2][1] =ys - y1;
       }


       //Read Orientations
       FILE *fd, *fk, *fo;
       int NrOrientations,TotalDiffrSpots;
       fd = fopen(fnDS,"r");
       fk = fopen(fnKey,"r");
       fo = fopen(fnOr,"r");
       fgets(line,1000,fk);
       sscanf(line,"%d",&NrOrientations);
       int **NrSpots;
       NrSpots = allocMatrixInt(NrOrientations,2);
       TotalDiffrSpots=0;
       for (i=0;i<NrOrientations;i++){
           fgets(line,1000,fk);
           sscanf(line,"%d",&NrSpots[i][0]);
           TotalDiffrSpots+=NrSpots[i][0];
           NrSpots[i][1] = TotalDiffrSpots - NrSpots[i][0];
       }
       double **SpotsMat;
       SpotsMat = allocMatrix(TotalDiffrSpots,3);
       for (i=0;i<TotalDiffrSpots;i++){
           fgets(line,1000,fd);
           sscanf(line,"%lf %lf %lf",&SpotsMat[i][0],&SpotsMat[i][1],&SpotsMat[i][2]);
       }
       double **OrientationMatrix;
       OrientationMatrix = allocMatrix(NrOrientations,9);
       for (i=0;i<NrOrientations;i++){
           fgets(line,1000,fo);
           sscanf(line,"%lf %lf %lf %lf %lf %lf %lf %lf %lf",&OrientationMatrix[i][0],&OrientationMatrix[i][1],&OrientationMatrix[i][2],&OrientationMatrix[i][3],&OrientationMatrix[i][4],&OrientationMatrix[i][5],&OrientationMatrix[i][6],&OrientationMatrix[i][7],&OrientationMatrix[i][8]);
       }


//       fclose(fileParam);

       int rc = FitOrientation_Calc(rown, gs, params.px, params.tx, params.ty, params.tz,
                         /*7*/params.nLayers, params.Lsd, XY, NrOrientations,
                         /*11*/NrSpots, OrientationMatrix, SpotsMat,
                         /*14*/nrFiles, params.OmegaStart, params.OmegaStep, SizeObsSpots,
                         /*18*/params.ybc, params.zbc, ObsSpotsInfo, params.minFracOverlap,
                         /*22*/params.LatticeConstant, params.Wavelength, params.nRings, params.ExcludePoleAngle,
                         /*26*/params.RingNumbers, params.OmegaRanges, params.NoOfOmegaRanges, params.BoxSizes,
                         params.tol, TotalDiffrSpots, xs, ys, result);

       assert(rc == 1);
       return 1;
}

int FitOrientation_Calc(int rown, double gs, double px, double tx, double ty, double tz,
                       int nLayers, double *Lsd, double **XY, int NrOrientations,
                       int **NrSpots, double **OrientationMatrix, double **SpotsMat,
                       int nrFiles, double OmegaStart, double OmegaStep, long long int SizeObsSpots,
                       double *ybc, double *zbc, int *ObsSpotsInfo, double minFracOverlap,
                       double LatticeConstant, int Wavelength, int nRings, double ExcludePoleAngle,
                       int *RingNumbers, double OmegaRanges[MAX_N_OMEGA_RANGES][2], int NoOfOmegaRanges,
                       double BoxSizes[MAX_N_OMEGA_RANGES][4],
                       double tol, int TotalDiffrSpots, double xs, double ys, double result[4])
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
    //FreeMemMatrix(SpotsMat,TotalDiffrSpots);
    //FreeMemMatrixInt(NrSpots,NrOrientations);
    //free(ObsSpotsInfo);
    //FreeMemMatrix(XY,3);
    //FreeMemMatrix(OrientationMatrix,NrOrientations);
    //FreeMemMatrix(OrientMatrix,MAX_POINTS_GRID_GOOD);
    clock_t end = clock();
    //diftotal = ((double)(startthis-start))/CLOCKS_PER_SEC;
    //printf("Time elapsed in reading bin files: %f [s]\n",diftotal);
    //diftotal = ((double)(startthis2-startthis))/CLOCKS_PER_SEC;
    //printf("Time elapsed in reading orientations: %f [s]\n",diftotal);
    double diftotal = ((double)(end-startthis2))/CLOCKS_PER_SEC;
    //printf("Time elapsed in comparing diffraction spots: %f [s]\n",diftotal);
    printf("%d %d %f %f %f %f %f %f %f\n",rown, OrientationGoodID, diftotal,
           xs, ys, BestEuler[0],BestEuler[1],BestEuler[2],BestFrac);
           
    result[0] = BestEuler[0];
    result[1] = BestEuler[1];
    result[2] = BestEuler[2];
    result[3] = BestFrac;
    return 1;
}
