
#include <config.h>

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#if HAVE_OPENMP == 1
#include <omp.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <nlopt.h>

#include "checks.h"
#include "Debug.h"
#include "parameters.h"
#include "SharedFuncsFit.h"
#include "FitOrientation.h"

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
	int i, j;
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
    CalcOverlapAccOrient(NrOfFiles,nLayers,ExcludePoleAngle,Lsd,SizeObsSpots,XGrain,
		YGrain,RotMatTilts,OmegaStart,OmegaStep,px,ybc,zbc,gs,hkls,n_hkls,
		Thetas,OmegaRanges,NoOfOmegaRanges,BoxSizes,P0,NrPixelsGrid,
		ObsSpotsInfo,OrientMatIn,&FracOverlap);
    return (1 - FracOverlap);
}

void
FitOrientation(
    const int NrOfFiles,
    const int nLayers,
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
    int n_hkls)
{
	unsigned n;
    long int i,j;
    n  = 3;
    double x[n],xl[n],xu[n];
    for( i=0; i<n; i++)
    {
        x[i] = EulerIn[i];
        xl[i] = x[i] - (tol*M_PI/180);
        xu[i] = x[i] + (tol*M_PI/180);
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
		for (j=0;j<3;j++){
			f_data.RotMatTilts[i][j] = RotMatTilts[i][j];
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
	double minf=1;
	nlopt_optimize(opt, x, &minf);
	nlopt_destroy(opt);
    *ResultFracOverlap = minf;
    *EulerOutA = x[0];
    *EulerOutB = x[1];
    *EulerOutC = x[2];
}

static bool fit_orientation_initialized = false;
static int result_fd = -1;
static struct parameters params;

static inline void
Init_FitOrientation(const char *ParamFN)
{
    if (fit_orientation_initialized)
        return;

    int result = parameters_read(ParamFN, &params);
    printf("parameters_read() OK. %i\n", result);
    fflush(stdout);
    
    char result_filename[1024];
    sprintf(result_filename, "%s/%s",
                             params.direct, "microstructure.mic");
    printf("Init_FitOrientation (result_filename=%s)\n",
                                 result_filename);
    result_fd = open(result_filename, O_CREAT|O_WRONLY,
                                      S_IRUSR|S_IWUSR);
    if (result_fd <= 0)
    {
      perror(result_filename);
      file_not_writable(result_filename);
    }
    fit_orientation_initialized = true;
    PROFILE_REPORT;
    PROFILE_RESET;
}

static bool ReadGridFile(const char *DataDirectory, int rown,
                         int *TotalNrSpots, double ***XY,
                         double *xs_out, double *ys_out, double *gs_out);

static bool ReadKey(const char *DataDirectory,
                    int *NrOrientations, int *TotalDiffrSpots, int ***NrSpots);

static bool ReadOrientations(const char *DataDirectory, int NrOrientations,
                             double ***OrientationMatrix);

static bool ReadSpots(const char *DataDirectory, int TotalDiffrSpots, double ***SpotsMat);

int FitOrientationAll(const char *ParamFN, int rown, const char *MicrostructureFN)
{
    printf("FitOrientationAll(%s,%i,%s)...\n", ParamFN, rown, MicrostructureFN);

   
    Init_FitOrientation(ParamFN);
    printf("Init_FitOrientation() OK.\n");
    fflush(stdout);

    double MaxTtheta = rad2deg*atan(params.MaxRingRad/params.Lsd[0]);
    //Read bin files
    char fn[1000];
    sprintf(fn,"%s/%s",params.direct,params.fn);
    int nrFiles,nrPixels;
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
    ReadCode = ReadBinFiles(fn,params.ext,params.StartNr,params.EndNr,ObsSpotsInfo,
                            params.nLayers,SizeObsSpots);
    if (ReadCode == 0){
        printf("Reading bin files did not go well. Please check.\n");
        return 0;
    }

    // Read *.txt files
    bool b;

    // Read Grid
    int TotalNrSpots;
    double **XY;
    double xs,ys,gs;
    b = ReadGridFile(params.direct, rown, &TotalNrSpots, &XY, &xs, &ys, &gs);
    assert(b);

    // Read Key
    int **NrSpots;
    int NrOrientations;
    int TotalDiffrSpots;
    b = ReadKey(params.direct, &NrOrientations, &TotalDiffrSpots, &NrSpots);
    assert(b);

    // Read Orientations
    double **OrientationMatrix;
    b = ReadOrientations(params.direct, NrOrientations, &OrientationMatrix);
    assert(b);

    // Read Spots
    double **SpotsMat;
    b = ReadSpots(params.direct, TotalDiffrSpots, &SpotsMat);
    assert(b);

    int rc = FitOrientation_Calc(rown, gs, params.px, params.tx, params.ty, params.tz,
                                 /*7*/params.nLayers, params.Lsd, XY, NrOrientations,
                                 /*11*/NrSpots, OrientationMatrix, SpotsMat,
                                 /*14*/nrFiles, params.OmegaStart, params.OmegaStep, SizeObsSpots,
                                 /*18*/params.ybc, params.zbc, ObsSpotsInfo, params.minFracOverlap,
                                 /*22*/params.LatticeConstant, params.Wavelength, params.SpaceGroup, params.ExcludePoleAngle,
                                 /*26*/params.OmegaRanges, params.NoOfOmegaRanges, params.BoxSizes,
                                 params.tol, TotalDiffrSpots, xs, ys, MaxTtheta);

    assert(rc == 1);
    return 1;
}

static bool ReadGridFile(const char *DataDirectory, int rown,
                         int *TotalNrSpots, double ***XY,
                         double *xs_out, double *ys_out, double *gs_out)
{
    char fnG[1024];
    char line[1024];

    sprintf(fnG,"%s/grid.txt", DataDirectory);
    LOG("reading: %s\n", fnG);
    FILE *fp = fopen(fnG,"r");
    if (fp == NULL) file_not_found(fnG);

    fgets(line,1000,fp);
    int n;
    n = sscanf(line,"%d",TotalNrSpots);
    assert(n == 1);
    if (rown > *TotalNrSpots){
        printf("Error: Grid point number greater than total number of grid points.\n");
        return false;
    }
    int counter=0;
    double y1,y2,xs,ys,gs;

    double **M = allocMatrix(3,3);
    while(counter<rown){
        fgets(line,1000,fp);
        counter+=1;
    }
    n = sscanf(line,"%lf %lf %lf %lf %lf",&y1,&y2,&xs,&ys,&gs);
    assert(n == 5);
    fclose(fp);
    LOG("closed: %s\n", fnG);
    if (y1>y2){
        M[0][0] =xs;
        M[0][1] =ys - y1;
        M[1][0] =xs - gs;
        M[1][1] =ys + y2;
        M[2][0] =xs + gs;
        M[2][1] =ys + y2;
    }
    else{
        M[0][0] =xs;
        M[0][1] =ys + y2;
        M[1][0] =xs - gs;
        M[1][1] =ys - y1;
        M[2][0] =xs + gs;
        M[2][1] =ys - y1;
    }

    // Set outputs...
    *XY = M;
    *xs_out = xs;
    *ys_out = ys;
    *gs_out = gs;
    return true;
}

static bool ReadKey(const char *DataDirectory,
                    int *NrOrientations, int *TotalDiffrSpots, int ***NrSpots)
{
    char line[1024];
    char fnKey[1024];
    sprintf(fnKey,"%s/Key.txt",DataDirectory);
    LOG("reading: %s\n", fnKey);
    FILE *fk = fopen(fnKey,"r");
    if (fk == NULL) file_not_found(fnKey);
    fgets(line,1000,fk);
    int n = sscanf(line,"%d",NrOrientations);
    assert(n == 1);
    int **M = allocMatrixInt(*NrOrientations,2);
    int tds=0;
    for (int i=0;i<*NrOrientations;i++){
        fgets(line,1000,fk);
        int n = sscanf(line,"%d",&M[i][0]);
        assert(n == 1);
        tds+=M[i][0];
        M[i][1] = tds - M[i][0];
    }
    fclose(fk);

    // Set outputs...
    *TotalDiffrSpots = tds;
    *NrSpots = M;
    return true;
}

static bool ReadOrientations(const char *DataDirectory, int NrOrientations, double ***OrientationMatrix)
{
    char fnOr[1024];
    sprintf(fnOr,"%s/OrientMat.txt",params.direct);
    char line[1024];
    LOG("reading: %s\n", fnOr);
    FILE *fo = fopen(fnOr,"r");
    if (fo == NULL) file_not_found(fnOr);

    double **M = allocMatrix(NrOrientations,9);

    for (int i=0;i<NrOrientations;i++){
        fgets(line,1000,fo);
        int n = sscanf(line,"%lf %lf %lf %lf %lf %lf %lf %lf %lf",&M[i][0],&M[i][1],&M[i][2],&M[i][3],&M[i][4],&M[i][5],&M[i][6],&M[i][7],&M[i][8]);
        assert(n == 9);
    }

    *OrientationMatrix = M;
    fclose(fo);
    LOG("closed: %s\n", fnOr);
    return true;
}

static bool ReadSpots(const char *DataDirectory, int TotalDiffrSpots, double ***SpotsMat)
{
    char fnDS[1000];
    sprintf(fnDS,"%s/DiffractionSpots.txt",params.direct);
    LOG("reading: %s\n", fnDS);
    FILE *fd = fopen(fnDS,"r");
    if (fd == NULL) file_not_found(fnDS);
    char line[1024];
    double **M = allocMatrix(TotalDiffrSpots,3);
    LOG("TotalDiffrSpots: %i\n", TotalDiffrSpots);
    for (int i=0;i<TotalDiffrSpots;i++){
        fgets(line,1000,fd);
        // printf("read: %i: %s", i, line);
        int count = sscanf(line,"%lf %lf %lf",&M[i][0],&M[i][1],&M[i][2]);
        assert(count == 3);
    }
    LOG("TotalDiffrSpots: done.\n");
    fclose(fd);

    // Set outputs:
    *SpotsMat = M;

    return true;
}

struct output_result
{
    double rown;
    double best[4];
};

int FitOrientation_Calc(int rown, double gs, double px, double tx, double ty, double tz,
                       /*7*/int nLayers, double *Lsd, double **XY, int NrOrientations,
                       /*11*/int **NrSpots, double **OrientationMatrix, double **SpotsMat,
                       /*14*/int nrFiles, double OmegaStart, double OmegaStep, long long int SizeObsSpots,
                       /*18*/double *ybc, double *zbc, int *ObsSpotsInfo, double minFracOverlap,
                       double LatticeConstant[6], double Wavelength, int SpaceGroup, double ExcludePoleAngle,
                       double OmegaRanges[MAX_N_OMEGA_RANGES][2], int NoOfOmegaRanges,
                       double BoxSizes[MAX_N_OMEGA_RANGES][4],
                       double tol, int TotalDiffrSpots, double xs, double ys, double MaxTtheta)
{
   // Go through each orientation and compare with observed spots.

  printf("FitOrientation_Calc()...\n");
       

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
    printf("%i: %i %f %f\n", nrFiles, nLayers, px, gs);
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
            // printf("ThrSps: %f %f %f\n", ThrSps[m][0], ThrSps[m][1], ThrSps[m][2]);
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

    double BestFrac = 0.0, BestEuler[3];
    for (int i = 0; i < 3; i++)
      BestEuler[i] = 0.0;
    // printf("Start fit...\n");

    if (OrientationGoodID>0){
		int n_hkls = 0;
		int hkls[5000][4];
		double Thetas[5000];
		for (int i=0;i<5000;i++){
			hkls[i][0] = 0;
			hkls[i][1] = 0;
			hkls[i][2] = 0;
			hkls[i][3] = 0;
			Thetas[i] = 0;
		}
                // printf("GenerateRingInfo...\n");
		int rc = GenerateRingInfo(SpaceGroup,LatticeConstant[0],LatticeConstant[1],
			     LatticeConstant[2],LatticeConstant[3],LatticeConstant[4],
			     LatticeConstant[5],Wavelength,MaxTtheta,Thetas,hkls,&n_hkls);
		assert(rc == 1);
                // printf("GenerateRingInfo done.\n");
        double Fractions, EulerIn[3], OrientIn[3][3], FracOut, EulerOutA, EulerOutB,EulerOutC,OMTemp[9];
        BestFrac = -1;

        // Parallel loop around FitOrientation...
        int i;
        for (i = 0; i < 3; i++)
          BestEuler[i] = 0.0;

        // #pragma omp parallel private(i,BestEuler,OMTemp,OrientIn,EulerIn,FracOut,Fractions)
        {
          // int num_threads = omp_get_num_threads();
          // printf("num_threads: %i\n", num_threads);
          // #pragma omp parallel for
        for (i=0;i<OrientationGoodID;i++){
          int k=0;
            for (k=0;k<9;k++){
                OMTemp[k] = OrientMatrix[i][k];
            }
            Convert9To3x3(OMTemp,OrientIn);
            OrientMat2Euler(OrientIn,EulerIn);
            FitOrientation(nrFiles,nLayers,ExcludePoleAngle,Lsd,SizeObsSpots,
				XG,YG,RotMatTilts,OmegaStart,OmegaStep,px,ybc,zbc,gs,
				OmegaRanges,NoOfOmegaRanges,BoxSizes,P0,NrPixelsGrid,
				ObsSpotsInfo,EulerIn,tol,&EulerOutA,&EulerOutB,
				&EulerOutC,&FracOut,hkls,Thetas,n_hkls);
            Fractions = 1-FracOut;
            
            if (Fractions > BestFrac){
              // printf("Fractions %f %i %i\n", Fractions, i, OrientationGoodID);
                BestFrac = Fractions;
                BestEuler[0] = EulerOutA;
                BestEuler[1] = EulerOutB;
                BestEuler[2] = EulerOutC;
            }
        }
        } // End of parallel section
        
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
    double result[5];
    result[0] = (double) rown;
    result[1] = BestEuler[0];
    result[2] = BestEuler[1];
    result[3] = BestEuler[2];
    result[4] = BestFrac;
    // fwrite(result, sizeof(double), 5, result_fd);
    printf("FitOrientation_Calc() pwrite: rown=%i\n",rown-1);
    int rc = pwrite(result_fd, result, 5*sizeof(double), (rown-1)*5*sizeof(double));
    if (rc < 0)
    {
        perror("result data file");
        exit(EXIT_FAILURE);
    }
    printf("FitOrientation_Calc() done.\n");
    
    return 1;
}
