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

#define RealType double
#define float32_t float
#define SetBit(A,k)   (A[(k/32)] |=  (1 << (k%32)))
#define ClearBit(A,k) (A[(k/32)] &= ~(1 << (k%32)))
#define TestBit(A,k)  (A[(k/32)] &   (1 << (k%32)))
#define deg2rad 0.0174532925199433
#define rad2deg 57.2957795130823
#define EPS 1E-5
#define MAX_N_SPOTS 200
#define MAX_N_OMEGA_RANGES 20

double**
allocMatrixF(int nrows, int ncols)
{
    double** arr;
    int i;
    arr = malloc(nrows * sizeof(*arr));
    if (arr == NULL ) {
        return NULL;
    }
    for ( i = 0 ; i < nrows ; i++) {
        arr[i] = malloc(ncols * sizeof(*arr[i]));
        if (arr[i] == NULL ) {
            return NULL;
        }
    }
    return arr;
}

int**
allocMatrixIntF(int nrows, int ncols)
{
    int** arr;
    int i;
    arr = malloc(nrows * sizeof(*arr));
    if (arr == NULL ) {
        return NULL;
    }
    for ( i = 0 ; i < nrows ; i++) {
        arr[i] = malloc(ncols * sizeof(*arr[i]));
        if (arr[i] == NULL ) {
            return NULL;
        }
    }
    return arr;
}

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
	f_data.P0 = allocMatrixF(nLayers,3);
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
	double tole = 1e-3;
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

int
main(int argc, char *argv[])
{
    clock_t start, end;
    double diftotal;
    start = clock();
    
    // Read params file.
    char *ParamFN;
    FILE *fileParam;
    ParamFN = argv[1];
    char aline[1000];
    struct TParam * Param1;
    fileParam = fopen(ParamFN,"r");
    char *str, dummy[1000];
    int LowNr,nLayers;
    double tx,ty,tz;
    while (fgets(aline,1000,fileParam)!=NULL){
        str = "nDistances ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %d", dummy, &nLayers);
            break;
        }
    }
    rewind(fileParam);
    double Lsd[nLayers],ybc[nLayers],zbc[nLayers],ExcludePoleAngle,LatticeConstant,Wavelength, minFracOverlap,doubledummy;
    double px, OmegaStart,OmegaStep,tol;
    int RingNumbers[20];
	char fn[1000];
	char fn2[1000];
	char direct[1000];
    double OmegaRanges[MAX_N_OMEGA_RANGES][2], BoxSizes[MAX_N_OMEGA_RANGES][4];
    int cntr=0,countr=0,conter=0,nRings=0,StartNr,EndNr,intdummy;
    int NoOfOmegaRanges=0;
    while (fgets(aline,1000,fileParam)!=NULL){
		str = "ReducedFileName ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %s", dummy, fn2);
            continue;
        }
		str = "DataDirectory ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %s", dummy, direct);
            continue;
        }
        str = "Lsd ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &Lsd[cntr]);
            cntr++;
            continue;
        }
        str = "RingNr ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %d", dummy, &RingNumbers[nRings]);
            nRings++;
            continue;
        }
        str = "StartNr ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %d", dummy, &StartNr);
            continue;
        }
        str = "EndNr ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %d", dummy, &EndNr);
            continue;
        }
        str = "ExcludePoleAngle ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &ExcludePoleAngle);
            continue;
        }
        str = "LatticeParameter ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &LatticeConstant);
            continue;
        }
        str = "tx ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &tx);
            continue;
        }
        str = "ty ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &ty);
            continue;
        }
        str = "BC ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf %lf", dummy, &ybc[conter], &zbc[conter]);
            conter++;
            continue;
        }
        str = "tz ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &tz);
            continue;
        }
        str = "OrientTol ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &tol);
            continue;
        }
        str = "MinFracAccept ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &minFracOverlap);
            continue;
        }
        str = "OmegaStart ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &OmegaStart);
            continue;
        }
        str = "OmegaStep ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &OmegaStep);
            continue;
        }
        str = "Wavelength ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &Wavelength);
            continue;
        }
        str = "px ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &px);
            continue;
        }
        str = "OmegaRange ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf %lf", dummy, &OmegaRanges[NoOfOmegaRanges][0],&OmegaRanges[NoOfOmegaRanges][1]);
            NoOfOmegaRanges++;
            continue;
        }
        str = "BoxSize ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf %lf %lf %lf", dummy, &BoxSizes[countr][0], &BoxSizes[countr][1], &BoxSizes[countr][2], &BoxSizes[countr][3]);
            countr++;
            continue;
        }
    }
    fclose(fileParam);
    //Read bin files
    char fnG[1000];
    sprintf(fnG,"%s/grid.txt",direct);
    char fnDS[1000];
    char fnKey[1000];
    char fnOr[1000];
    sprintf(fnDS,"%s/DiffractionSpots.txt",direct);
    sprintf(fnKey,"%s/Key.txt",direct);
    sprintf(fnOr,"%s/OrientMat.txt",direct);
    sprintf(fn,"%s/%s",direct,fn2);
    int i,j,m,nrFiles,nrPixels;
    char *ext="bin";
    int *ObsSpotsInfo;
    int ReadCode;
    nrFiles = EndNr - StartNr + 1;
    nrPixels = 2048*2048;
    long long int SizeObsSpots;
    SizeObsSpots = (nLayers);
    SizeObsSpots*=nrPixels;
    SizeObsSpots*=nrFiles;
    SizeObsSpots/=32;
    ObsSpotsInfo = malloc(SizeObsSpots*sizeof(ObsSpotsInfo));
    if (ObsSpotsInfo==NULL){
        printf("Could not allocate ObsSpotsInfo.\n");
        return 0;
    }
    ReadCode = ReadBinFiles(fn,ext,StartNr,EndNr,ObsSpotsInfo,nLayers,SizeObsSpots);
    if (ReadCode == 0){
        printf("Reading bin files did not go well. Please check.\n");
        return 0;
    }
    
    //Read position.
    int rown=atoi(argv[2]);
    FILE *fp;
    fp = fopen(fnG,"r");
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
    XY = allocMatrixF(3,3);
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
    double GridSize=2*gs;

    //Read Orientations
    clock_t startthis;
    startthis = clock();
    FILE *fd, *fk, *fo;
    int NrOrientations,TotalDiffrSpots;
    fd = fopen(fnDS,"r");
    fk = fopen(fnKey,"r");
    fo = fopen(fnOr,"r");
    fgets(line,1000,fk);
    sscanf(line,"%d",&NrOrientations);
    int **NrSpots;
    NrSpots = allocMatrixIntF(NrOrientations,2);
    TotalDiffrSpots=0;
    for (i=0;i<NrOrientations;i++){
        fgets(line,1000,fk);
        sscanf(line,"%d",&NrSpots[i][0]);
        TotalDiffrSpots+=NrSpots[i][0];
        NrSpots[i][1] = TotalDiffrSpots - NrSpots[i][0];
    }
    double **SpotsMat;
    SpotsMat = allocMatrixF(TotalDiffrSpots,3);
    for (i=0;i<TotalDiffrSpots;i++){
        fgets(line,1000,fd);
        sscanf(line,"%lf %lf %lf",&SpotsMat[i][0],&SpotsMat[i][1],&SpotsMat[i][2]);
    }
    double **OrientationMatrix;
    OrientationMatrix = allocMatrixF(NrOrientations,9);
    for (i=0;i<NrOrientations;i++){
        fgets(line,1000,fo);
        sscanf(line,"%lf %lf %lf %lf %lf %lf %lf %lf %lf",&OrientationMatrix[i][0],&OrientationMatrix[i][1],&OrientationMatrix[i][2],&OrientationMatrix[i][3],&OrientationMatrix[i][4],&OrientationMatrix[i][5],&OrientationMatrix[i][6],&OrientationMatrix[i][7],&OrientationMatrix[i][8]);
    }

    FitOrientationAll();

    // Go through each orientation and compare with observed spots.
    clock_t startthis2;
    startthis2 = clock();
    int NrPixelsGrid=2*(ceil((gs*2)/px))*(ceil((gs*2)/px));
    int NrSpotsThis,StartingRowNr;
    double FracOverT;
    double RotMatTilts[3][3], OrientationMatThis[9], OrientationMatThisUnNorm[9];
    RotationTilts(tx,ty,tz,RotMatTilts);
    double **OrientMatrix;
    OrientMatrix = allocMatrixF(MAX_POINTS_GRID_GOOD,10);
    int OrientationGoodID=0;
    double MatIn[3],P0[nLayers][3],P0T[3];
    double OrientMatIn[3][3],XG[3],YG[3];
    double ThrSps[MAX_N_SPOTS][3];
    MatIn[0]=0;
    MatIn[1]=0;
    MatIn[2]=0;
    for (i=0;i<nLayers;i++){
        MatIn[0] = -Lsd[i];
        MatrixMultF(RotMatTilts,MatIn,P0T);
        for (j=0;j<3;j++){
            P0[i][j] = P0T[j];
        }
    }
    for (j=0;j<3;j++){
        XG[j] = XY[j][0];
        YG[j] = XY[j][1];
    }
    for (i=0;i<NrOrientations;i++){
        NrSpotsThis = NrSpots[i][0];
        StartingRowNr = NrSpots[i][1];
        m = 0;
        for (m=0;m<9;m++){
            OrientationMatThisUnNorm[m] = OrientationMatrix[i][m];
			if (OrientationMatThisUnNorm[m] == -0.0){
				OrientationMatThisUnNorm[m] = 0;
			}
        }
        m=0;
        NormalizeMat(OrientationMatThisUnNorm,OrientationMatThis);
        for (j=StartingRowNr;j<(StartingRowNr+NrSpotsThis);j++){
            ThrSps[m][0] = SpotsMat[j][0];
            ThrSps[m][1] = SpotsMat[j][1];
            ThrSps[m][2] = SpotsMat[j][2];
            m++;
        }
        Convert9To3x3(OrientationMatThis,OrientMatIn);
        CalcFracOverlap(nrFiles,nLayers,NrSpotsThis,ThrSps,OmegaStart,OmegaStep,XG,YG,Lsd,SizeObsSpots,RotMatTilts,px,ybc,zbc,
			gs,P0,NrPixelsGrid,ObsSpotsInfo,OrientMatIn,&FracOverT);
        if (FracOverT >= minFracOverlap){
            for (j=0;j<9;j++){
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
        for (i=0;i<OrientationGoodID;i++){
            for (j=0;j<9;j++){
                OMTemp[j] = OrientMatrix[i][j];
            }
            Convert9To3x3(OMTemp,OrientIn);
            OrientMat2Euler(OrientIn,EulerIn);
            FitOrientation(nrFiles,nLayers,LatticeConstant,Wavelength,nRings,ExcludePoleAngle,Lsd,SizeObsSpots,XG,YG,RotMatTilts,OmegaStart,OmegaStep,px,ybc,zbc,gs,RingNumbers,OmegaRanges,NoOfOmegaRanges,BoxSizes,P0,NrPixelsGrid,ObsSpotsInfo,EulerIn,tol,&EulerOutA,&EulerOutB,&EulerOutC,&FracOut);
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
    end = clock();
    //diftotal = ((double)(startthis-start))/CLOCKS_PER_SEC;
    //printf("Time elapsed in reading bin files: %f [s]\n",diftotal);
    //diftotal = ((double)(startthis2-startthis))/CLOCKS_PER_SEC;
    //printf("Time elapsed in reading orientations: %f [s]\n",diftotal);
    diftotal = ((double)(end-start))/CLOCKS_PER_SEC;
    //printf("Time elapsed in comparing diffraction spots: %f [s]\n",diftotal);
    printf("%d %d %f %f %f %f %f %f %f\n",atoi(argv[2]), OrientationGoodID, diftotal, xs, ys, BestEuler[0],BestEuler[1],BestEuler[2],BestFrac);
    return 0;
}
