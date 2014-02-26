//
//
//  
//
//  Created by Hemant Sharma on 11/13/13.
//
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define RealType double

void
HexGrid(
    double GridSize,
    double Rsample,
    double NrHex,
    double HtTriangle,
    double ALast,
    double **XY)
{
    int i,j;
    int counter=0;
    int NrRowElements;
    double ythis,xstart,ynext, ysmall, ybig;
    ysmall = HtTriangle*(1.0/3.0);
    ybig = HtTriangle*(2.0/3.0);
    for (i=-NrHex;i<=NrHex;i++){
        if (i == 0){
            continue;
        }
        if (i<0){
            ynext = ybig;
        }
        else {
            ynext = ysmall;
        }
        NrRowElements = (2*((2*NrHex)-(fabs(i))))+1;
        ythis = HtTriangle*i;
        xstart = -ALast + (fabs(i)*GridSize*0.5);
        for (j=0;j<NrRowElements;j++)
        {
            XY[counter][0] = ynext;
            if (ynext==ybig){
                XY[counter][1] = ysmall;
            }
            else{
                XY[counter][1] = ybig;
            }
            XY[counter][2] = xstart + (GridSize*j)/2;
            XY[counter][3] = ythis - (ynext*i/(fabs(i)));
            XY[counter][4] = GridSize/2;
            counter = counter + 1;
            if (ynext==ybig){
                ynext = ysmall;
            }
            else{
                ynext = ybig;
            }
        }
    }
}

double**
allocMatrix(int nrows, int ncols)
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

void
FreeMemMatrix(
              RealType **mat,
              int nrows)
{
    int r;
    for ( r = 0 ; r < nrows ; r++) {
        free(mat[r]);
    }
    free(mat);
}

int
main(int argc, char *argv[])
{
    clock_t start, end;
    double diftotal;
    start = clock();
    
    char *ParamFN;
    FILE *fileParam;
    ParamFN = argv[1];
    char aline[1000];
    fileParam = fopen(ParamFN,"r");
    char *str, dummy[1000];
    int LowNr;
    double GridSize, Rsample;
    while (fgets(aline,1000,fileParam)!=NULL){
        str = "GridSize ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &GridSize);
            continue;
        }
        str = "Rsample ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &Rsample);
            continue;
        }
    }
    fclose(fileParam);

    // Make grid.
    double NrHex, ALarge, ALast,HtTriangle;
    int i,j;
    int NrGridElements=0;
    FILE *fp;
    ALarge = (2*Rsample)/(sqrt(3));
    HtTriangle = ((sqrt(3))*(GridSize))/(2);
    NrHex = ceil(ALarge/GridSize);
    ALast = GridSize*NrHex;
    for (i=1;i<=NrHex;i++){
        NrGridElements+=2*((2*((2*NrHex)-i))+1);
    }
    double **XYGrid;
    XYGrid = allocMatrix(NrGridElements,5);
    HexGrid(GridSize,Rsample,NrHex,HtTriangle,ALast,XYGrid);
    printf("Number of grid points: %d.\n",NrGridElements);
    fp = fopen("grid.txt","w");
    if (fp==NULL){
        printf("Cannot open file, grid.txt\n");
        return (0);
    }
    fprintf(fp,"%d\n",NrGridElements);
    for (j=0;j<NrGridElements;j++){
        fprintf(fp,"%f %f %f %f %f\n",XYGrid[j][0],XYGrid[j][1],XYGrid[j][2],XYGrid[j][3],XYGrid[j][4]);
    }
    fclose(fp);
    FreeMemMatrix(XYGrid,NrGridElements);
    end = clock();
    diftotal = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Time elapsed in making HexGrid: %f [s]\n",diftotal);
    return 0;
}