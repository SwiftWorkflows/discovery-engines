
#include "checks.h"

#include "FitOrientationParametersNLOpt.c"

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
//    struct TParam * Param1;
    fileParam = fopen(ParamFN,"r");
    if (fileParam == NULL) file_not_found(ParamFN);
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
    double px, OmegaStart,OmegaStep,tol,lsdtol,tiltstol,bctol,lsdtolrel;
    int RingNumbers[20];
    char fn[1000],ext[1024];
    double OmegaRanges[MAX_N_OMEGA_RANGES][2], BoxSizes[MAX_N_OMEGA_RANGES][4];
    int cntr=0,countr=0,conter=0,nRings=0,StartNr,EndNr,intdummy;
    int NoOfOmegaRanges=0;
    while (fgets(aline,1000,fileParam)!=NULL){
		str = "ReducedFileName ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %s", dummy, fn);
            continue;
        }
		str = "extReduced ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %s", dummy, ext);
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
        str = "LsdTol ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &lsdtol);
            continue;
        }
        str = "LsdRelativeTol ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &lsdtolrel);
            continue;
        }
        str = "BCTol ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &bctol);
            continue;
        }
        str = "TiltsTol ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &tiltstol);
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
    int i,j,m,nrFiles,nrPixels;
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
    char *fnG="grid.txt";
    int rown=atoi(argv[2]);
    FILE *fp;
    fp = fopen(fnG,"r");
    if (fp == NULL) file_not_found(fnG);
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
    double GridSize=2*gs;

    //Read Orientations
    clock_t startthis;
    startthis = clock();
    FILE *fd, *fk, *fo;
    char *fnDS="DiffractionSpots.txt";
    char *fnKey="Key.txt";
    char *fnOr="OrientMat.txt";
    int NrOrientations,TotalDiffrSpots;
    fd = fopen(fnDS,"r");
    if (fd == NULL) file_not_found(fnDS);
    fk = fopen(fnKey,"r");
    if (fk == NULL) file_not_found(fnKey);
    fo = fopen(fnOr,"r");
    if (fo == NULL) file_not_found(fnOr);
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
        sscanf(line,"%lf %lf %lf %lf %lf %lf %lf %lf %lf",&OrientationMatrix[i][0],&OrientationMatrix[i][1],&OrientationMatrix[i][2],
			&OrientationMatrix[i][3],&OrientationMatrix[i][4],&OrientationMatrix[i][5],&OrientationMatrix[i][6],&OrientationMatrix[i][7],
			&OrientationMatrix[i][8]);
    }
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

    int resultMax = 6+nLayers*3;
    double result[resultMax];

    for (int i = 0; i < OrientationGoodID; i++)
      optimizeOrientation(OrientMatrix[i],
                          tx, ty, tz, nLayers,
                          nrFiles, ObsSpotsInfo,
                          LatticeConstant, Wavelength, nRings,
                          ExcludePoleAngle, Lsd, SizeObsSpots,
                          OmegaStart, OmegaStep, px, ybc,  zbc,
                          gs, RingNumbers, OmegaRanges, NoOfOmegaRanges,
                          BoxSizes, P0, NrPixelsGrid,
                          tol, lsdtol, lsdtolrel,
                          tiltstol,
                          bctol,
                          result,
                          resultMax);

    // Free memory
    FreeMemMatrix(SpotsMat,TotalDiffrSpots);
    FreeMemMatrixInt(NrSpots,NrOrientations);
    free(ObsSpotsInfo);
    FreeMemMatrix(XY,3);
    FreeMemMatrix(OrientationMatrix,NrOrientations);
    FreeMemMatrix(OrientMatrix,MAX_POINTS_GRID_GOOD);
    end = clock();
    diftotal = ((double)(startthis-start))/CLOCKS_PER_SEC;
    printf("Time elapsed in reading bin files: %f [s]\n",diftotal);
    diftotal = ((double)(startthis2-startthis))/CLOCKS_PER_SEC;
    printf("Time elapsed in reading orientations: %f [s]\n",diftotal);
    diftotal = ((double)(end-startthis2))/CLOCKS_PER_SEC;
    printf("Time elapsed in comparing diffraction spots: %f [s]\n",diftotal);

    return 0;
}
