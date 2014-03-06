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

#include "CalcDiffractionSpots.h"

#define RealType double
#define float32_t float
#define SetBit(A,k)   (A[(k/32)] |=  (1 << (k%32)))
#define ClearBit(A,k) (A[(k/32)] &= ~(1 << (k%32)))
#define TestBit(A,k)  (A[(k/32)] &   (1 << (k%32)))
#define deg2rad 0.0174532925199433
#define rad2deg 57.2957795130823
#define EPS 1e-5
#define MAX_N_SPOTS 200
#define MAX_N_OMEGA_RANGES 20
#define MAX_POINTS_GRID_GOOD 300000

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

int**
allocMatrixInt(int nrows, int ncols)
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

void
FreeMemMatrix(RealType **mat,int nrows)
{
    int r;
    for ( r = 0 ; r < nrows ; r++) {
        free(mat[r]);
    }
    free(mat);
}

void
FreeMemMatrixInt(int **mat,int nrows)
{
    int r;
    for ( r = 0 ; r < nrows ; r++) {
        free(mat[r]);
    }
    free(mat);
}

void
Convert9To3x3(double MatIn[9],double MatOut[3][3])
{
    int i,j,k=0;
    for (i=0;i<3;i++){
        for (j=0;j<3;j++){
            MatOut[i][j] = MatIn[k];
            k++;
        }
    }
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

void ReadHeader(
    FILE *fp,
    struct Theader * head)
{
    fread(&head->uBlockHeader,sizeof(uint32_t),1,fp);
    fread(&head->BlockType,sizeof(uint16_t),1,fp);
    fread(&head->DataFormat,sizeof(uint16_t),1,fp);
    fread(&head->NumChildren,sizeof(uint16_t),1,fp);
    fread(&head->NameSize,sizeof(uint16_t),1,fp);
    fread(&head->DataSize,sizeof(uint32_t),1,fp);
    fread(&head->ChunkNumber,sizeof(uint16_t),1,fp);
    fread(&head->TotalChunks,sizeof(uint16_t),1,fp);
    fread(&head->BlockName,(sizeof(char)*(head->NameSize)),1,fp);
}

int
ReadBinFiles(
    char FileStem[1000],
    char *ext,
    int StartNr,
    int EndNr,
    int *ObsSpotsMat,
    int nLayers,
    long long int ObsSpotsSize)
{
    int i,j,k,nElements,nCheck,ythis,zthis,NrOfFiles,NrOfPixels;
    long long int BinNr;
    long long int TempCntr;
    float32_t dummy;
    uint32_t dummy2;
    FILE *fp;
    char FileName[1024];
    struct Theader Header1;
    uint16_t *ys, *zs, *peakID;
    float32_t *intensity;
    int counter=0;
    NrOfFiles = EndNr - StartNr + 1;
    NrOfPixels = 2048*2048;
    for (k=0;k<ObsSpotsSize;k++){
        ObsSpotsMat[k] = 0;
    }
    for (k=0;k<nLayers;k++){
        for (i=StartNr;i<=EndNr;i++){
            sprintf(FileName,"%s_%06d.%s%d",FileStem,i,ext,k);
            printf("Reading file : %s\n",FileName);
            fp = fopen(FileName,"r");
            if (fp == NULL) file_not_found(FileName);
            fread(&dummy,sizeof(float32_t),1,fp);
            ReadHeader(fp,&Header1);
            fread(&dummy2,sizeof(uint32_t),1,fp);
            fread(&dummy2,sizeof(uint32_t),1,fp);
            fread(&dummy2,sizeof(uint32_t),1,fp);
            fread(&dummy2,sizeof(uint32_t),1,fp);
            fread(&dummy2,sizeof(uint32_t),1,fp);
            ReadHeader(fp,&Header1);
            nElements = (Header1.DataSize - Header1.NameSize)/2;
            ys = malloc(nElements*sizeof(*ys));
            zs = malloc(nElements*sizeof(*zs));
            peakID = malloc(nElements*sizeof(*peakID));
            intensity = malloc(nElements*sizeof(*intensity));
            fread(ys,sizeof(uint16_t)*nElements,1,fp);
            ReadHeader(fp,&Header1);
            nCheck = (Header1.DataSize - Header1.NameSize)/2;
            if (nCheck!=nElements){
                printf("Number of elements mismatch.\n");
                return 0;
            }
            fread(zs,sizeof(uint16_t)*nElements,1,fp);
            ReadHeader(fp,&Header1);
            nCheck = (Header1.DataSize - Header1.NameSize)/4;
            if (nCheck!=nElements){
                printf("Number of elements mismatch.\n");
                return 0;
            }
            fread(intensity,sizeof(float32_t)*nElements,1,fp);
            ReadHeader(fp,&Header1);
            nCheck = (Header1.DataSize - Header1.NameSize)/2;
            if (nCheck!=nElements){
                printf("Number of elements mismatch.\n");
                return 0;
            }
            fread(peakID,sizeof(uint16_t)*nElements,1,fp);
            for (j=0;j<nElements;j++){
                ythis=(int)ys[j];
                zthis=(int)zs[j];
                BinNr = k;
                BinNr *=NrOfFiles;
                BinNr *= NrOfPixels;
                TempCntr = (counter*(NrOfPixels));
                BinNr += TempCntr;
                BinNr += (ythis*(2048));
                BinNr += zthis;
                SetBit(ObsSpotsMat,BinNr);
            }
            fclose(fp);
            counter+=1;
        }
        counter = 0;
    }
    return 1;
    //    free(ys);
    //    free(zs);
    //    free(peakID);
    //    free(intensity);
}

double sin_cos_to_angle (double s, double c)
{
    return (s >= 0.0) ? acos(c) : 2.0 * M_PI - acos(c);
}

void OrientMat2Euler(
    double m[3][3],
    double Euler[3])
{
    double psi, phi, theta, sph;
	if (fabs(m[2][2] - 1.0) < EPS){
		phi = 0;
	}else{
	    phi = acos(m[2][2]);
	}
    sph = sin(phi);
    if (fabs(sph) < EPS)
    {
        psi = 0.0;
        theta = (fabs(m[2][2] - 1.0) < EPS) ? sin_cos_to_angle(m[1][0], m[0][0]) : sin_cos_to_angle(-m[1][0], m[0][0]);
    } else
    {
        psi = (fabs(-m[1][2] / sph) <= 1.0) ? sin_cos_to_angle(m[0][2] / sph, -m[1][2] / sph) : sin_cos_to_angle(m[0][2] / sph,1);
        theta = (fabs(m[2][1] / sph) <= 1.0) ? sin_cos_to_angle(m[2][0] / sph, m[2][1] / sph) : sin_cos_to_angle(m[2][0] / sph,1);
    }
    Euler[0] = psi;
    Euler[1] = phi;
    Euler[2] = theta;
}

void Euler2OrientMat(
    double Euler[3],
    double m_out[3][3])
{
    double psi, phi, theta, cps, cph, cth, sps, sph, sth;
    psi = Euler[0];
    phi = Euler[1];
    theta = Euler[2];
    cps = cos(psi) ; cph = cos(phi); cth = cos(theta);
    sps = sin(psi); sph = sin(phi); sth = sin(theta);
    m_out[0][0] = cth * cps - sth * cph * sps;
    m_out[0][1] = -cth * cph * sps - sth * cps;
    m_out[0][2] = sph * sps;
    m_out[1][0] = cth * sps + sth * cph * cps;
    m_out[1][1] = cth * cph * cps - sth * sps;
    m_out[1][2] = -sph * cps;
    m_out[2][0] = sth * sph;
    m_out[2][1] = cth * sph;
    m_out[2][2] = cph;
}

void
MatrixMultF33(
    RealType m[3][3],
    RealType n[3][3],
    RealType res[3][3])
{
    int r;
    for (r=0; r<3; r++) {
        res[r][0] = m[r][0]*n[0][0] + m[r][1]*n[1][0] + m[r][2]*n[2][0];
        res[r][1] = m[r][0]*n[0][1] + m[r][1]*n[1][1] + m[r][2]*n[2][1];
        res[r][2] = m[r][0]*n[0][2] + m[r][1]*n[1][2] + m[r][2]*n[2][2];
    }
}

void
RotationTilts(
    double tx,
    double ty,
    double tz,
    double RotMatOut[3][3])
{
    tx = deg2rad*tx;
    ty = deg2rad*ty;
    tz = deg2rad*tz;
    double r1[3][3];
    double r2[3][3];
    double r3[3][3];
    double r1r2[3][3];
    r1[0][0] = cos(tz);
    r1[0][1] = -sin(tz);
    r1[0][2] = 0;
    r1[1][0] = sin(tz);
    r1[1][1] = cos(tz);
    r1[1][2] = 0;
    r1[2][0] = 0;
    r1[2][1] = 0;
    r1[2][2] = 1;
    r2[0][0] = cos(ty);
    r2[0][1] = 0;
    r2[0][2] = sin(ty);
    r2[1][0] = 0;
    r2[1][1] = 1;
    r2[1][2] = 0;
    r2[2][0] = -sin(ty);
    r2[2][1] = 0;
    r2[2][2] = cos(ty);
    r3[0][0] = 1;
    r3[0][1] = 0;
    r3[0][2] = 0;
    r3[1][0] = 0;
    r3[1][1] = cos(tx);
    r3[1][2] = -sin(tx);
    r3[2][0] = 0;
    r3[2][1] = sin(tx);
    r3[2][2] = cos(tx);
    MatrixMultF33(r1,r2,r1r2);
    MatrixMultF33(r1r2,r3,RotMatOut);
}

void
DisplacementSpots(
    RealType a,
    RealType b,
    RealType xi,
    RealType yi,
    RealType zi,
    RealType omega,
    RealType *Displ_y,
    RealType *Displ_z)
{
    RealType lenInv = 1/sqrt(xi*xi + yi*yi + zi*zi);
    xi = xi*lenInv;
    yi = yi*lenInv;
    zi = zi*lenInv;

    RealType OmegaRad = deg2rad * omega;
    RealType sinOme = sin(OmegaRad);
    RealType cosOme = cos(OmegaRad);
    RealType t = (a*cosOme - b*sinOme)/xi;

    *Displ_y = ((a*sinOme)+(b*cosOme))-(t*yi);
    *Displ_z = -t*zi;
}

int sign(int x){
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

struct Point2D {
    int x, y;
};


int orient2d(struct Point2D a, struct Point2D b, struct Point2D c)
{
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}

double len2d(struct Point2D a, struct Point2D b, struct Point2D c)
{
    return fabs(((b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x)))/sqrt(((a.y-b.y)*(a.y-b.y))+((b.x-a.x)*(b.x-a.x)));
}

void
CalcPixels2(double Edges[3][2], int **Pixels, int *counter)
{
    int i;
    double minX=10000000,maxX=-10000000,minY=100000000,maxY=-100000000;
    for (i=0;i<3;i++){
        Edges[i][0] = round(Edges[i][0]);
        Edges[i][1] = round(Edges[i][1]);
    }
    for (i=0;i<3;i++){
        if (Edges[i][0]<minX){
            minX = Edges[i][0];
        }
        if (Edges[i][0]>maxX){
            maxX = Edges[i][0];
        }
        if (Edges[i][1]<minY){
            minY = Edges[i][1];
        }
        if (Edges[i][1]>maxY){
            maxY = Edges[i][1];
        }
    }
    *counter = 0;
    int A01 = Edges[0][1] - Edges[1][1], B01 = Edges[1][0] - Edges[0][0];
    int A12 = Edges[1][1] - Edges[2][1], B12 = Edges[2][0] - Edges[1][0];
    int A20 = Edges[2][1] - Edges[0][1], B20 = Edges[0][0] - Edges[2][0];
    struct Point2D p = { minX, minY };
    struct Point2D v0 = { Edges[0][0], Edges[0][1]};
    struct Point2D v1 = { Edges[1][0], Edges[1][1]};
    struct Point2D v2 = { Edges[2][0], Edges[2][1]};
    int w0_row = orient2d(v1, v2, p);
    int w1_row = orient2d(v2, v0, p);
    int w2_row = orient2d(v0, v1, p);
    for (p.y = minY; p.y <= maxY; p.y++) {
        int w0 = w0_row;
        int w1 = w1_row;
        int w2 = w2_row;
        for (p.x = minX; p.x <= maxX; p.x++) {
            if (w0 >= 0 && w1 >= 0 && w2 >= 0){
                Pixels[*counter][0] = p.x;
                Pixels[*counter][1] = p.y;
                *counter+=1;
            }
            else if(len2d(v1,v2,p)<0.99){
                Pixels[*counter][0] = p.x;
                Pixels[*counter][1] = p.y;
                *counter+=1;
            }
            else if(len2d(v2,v0,p)<0.99){
                Pixels[*counter][0] = p.x;
                Pixels[*counter][1] = p.y;
                *counter+=1;
            }
            else if(len2d(v0,v1,p)<0.99){
                Pixels[*counter][0] = p.x;
                Pixels[*counter][1] = p.y;
                *counter+=1;
            }
            w0 += A12;
            w1 += A20;
            w2 += A01;
        }
        w0_row += B12;
        w1_row += B20;
        w2_row += B01;
    }
}

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
    double *FracOver)
{
    int j,OmeBin,OutofBounds,k,l;
    int nSpotsReduced=nTspots;
    double OmegaThis,ythis,zthis,XGT,YGT,Displ_Y,Displ_Z,ytemp,ztemp,xyz[3],P1[3],ABC[3],outxyz[3],YZSpots[3][2],Lsd,ybc,zbc,P0[3];
    int **InPixels,NrInPixels, OverlapPixels,Layer;
    long long int BinNr;
    double SpotsOverAll=0;
    *FracOver = 0;
    InPixels = allocMatrixInt(NrPixelsGrid,2);
    for (Layer=0;Layer<nLayers;Layer++){
        nSpotsReduced=nTspots;
        Lsd = Lsds[Layer];
        ybc = ybcs[Layer];
        zbc = zbcs[Layer];
        for (k=0;k<3;k++){
            P0[k] = P0All[Layer][k];
        }
        for (j=0;j<nTspots;j++){
            ythis = (TheorSpots[j][0])*(Lsd/Lsds[0]);
            zthis = (TheorSpots[j][1])*(Lsd/Lsds[0]);
            OmegaThis = TheorSpots[j][2];
            OmeBin = (int) floor((-OmegaStart + OmegaThis)/OmegaStep);
            OutofBounds = 0;
            for (k=0;k<3;k++){
                XGT = XGrain[k];
                YGT = YGrain[k];
                DisplacementSpots(XGT,YGT,Lsd,ythis,zthis,OmegaThis,&Displ_Y,&Displ_Z);
                ytemp = -(ythis + Displ_Y);
                ztemp =  (zthis + Displ_Z);
                xyz[0] = 0;
                xyz[1] = ytemp;
                xyz[2] = ztemp;
                MatrixMultF(RotMatTilts,xyz,P1);
                for (l=0;l<3;l++){
                    ABC[l] = P1[l]-P0[l];
                }
                outxyz[0] = 0;
                outxyz[1] = P0[1] -(ABC[1]*P0[0])/(ABC[0]);
                outxyz[2] = P0[2] -(ABC[2]*P0[0])/(ABC[0]);
                YZSpots[k][0] = (outxyz[1])/px + ybc;
                YZSpots[k][1] = (outxyz[2])/px + zbc;
                if (YZSpots[k][0] > 2048){
                    OutofBounds=1;
                    continue;
                }
                else if (YZSpots[k][0] < 0){
                    OutofBounds=1;
                    continue;
                }
                if (YZSpots[k][1] > 2048){
                    OutofBounds=1;
                    continue;
                }
                else if (YZSpots[k][1] < 0){
                    OutofBounds=1;
                    continue;
                }
            }
            if (OutofBounds==1){
                nSpotsReduced--;
                continue;
            }
            if (gs*2 > px){
                CalcPixels2(YZSpots,InPixels,&NrInPixels);
            }else{
                InPixels[1][0] = (int) round((YZSpots[1][0]+YZSpots[2][0]+YZSpots[3][0])/3);
                InPixels[1][1] = (int) round((YZSpots[1][1]+YZSpots[2][1]+YZSpots[3][1])/3);
                NrInPixels = 1;
            }
            OverlapPixels = 0;
            for (k=0;k<NrInPixels;k++){
                BinNr = Layer*NrOfFiles;
                BinNr *= 2048;
                BinNr *= 2048;
                BinNr += OmeBin*2048*2048;
                BinNr += InPixels[k][0]*2048;
                BinNr += InPixels[k][1];
                if (TestBit(ObsSpotsInfo,BinNr)){
                    OverlapPixels+=1;
                }
            }
            if (OverlapPixels>0){
                *FracOver+=(double)((double)OverlapPixels)/((double)NrInPixels);
            }
        }
        SpotsOverAll+=nSpotsReduced;
    }
    if (SpotsOverAll!=0){
        *FracOver/=(SpotsOverAll);
    }
    else{
        *FracOver=0;
    }
    FreeMemMatrixInt(InPixels,NrPixelsGrid);
}

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
    double *FracOverlap)
{
    int nTspots,i;
    double **TheorSpots, Lsd0=Lsd[0];
    TheorSpots = allocMatrix(MAX_N_SPOTS,3);
    CalcDiffractionSpots(LatticeConstant, Wavelength, Lsd0, nRings, ExcludePoleAngle, RingNumbers, OmegaRanges, NoOfOmegaRanges,
		BoxSizes, &nTspots, OrientMatIn,TheorSpots);
    double FracOver;
    double XG[3],YG[3],ThrSps[nTspots][3];
    for (i=0;i<3;i++){
        XG[i] = XGrain[i];
        YG[i] = YGrain[i];
    }
    for (i=0;i<nTspots;i++){
        ThrSps[i][0] = TheorSpots[i][0];
        ThrSps[i][1] = TheorSpots[i][1];
        ThrSps[i][2] = TheorSpots[i][2];
    }
    CalcFracOverlap(NrOfFiles,nLayers,nTspots,ThrSps,OmegaStart,OmegaStep,XG,YG,Lsd,SizeObsSpots,RotMatTilts,px,ybc,zbc,gs,
		P0,NrPixelsGrid,ObsSpotsInfo,OrientMatIn,&FracOver);
    *FracOverlap = FracOver;
    FreeMemMatrix(TheorSpots,MAX_N_SPOTS);
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
	double tole = 1e-3;
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
NormalizeMat(
    double OMIn[9],
    double OMOut[9])
{
    double determinant;
    int i;
    determinant = (OMIn[0]*((OMIn[4]*OMIn[8])-(OMIn[5]*OMIn[7])))
                 -(OMIn[1]*((OMIn[3]*OMIn[8])-(OMIn[5]*OMIn[6])))
                 +(OMIn[2]*((OMIn[3]*OMIn[7])-(OMIn[4]*OMIn[6])));
    for (i=0;i<9;i++){
        OMOut[i] = OMIn[i]/determinant;
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
  double EulerIn[3],OrientIn[3][3], FracOut, EulerOutA, EulerOutB,EulerOutC,BestFrac,BestEuler[3],XG[3],YG[3],OMTemp[9];
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