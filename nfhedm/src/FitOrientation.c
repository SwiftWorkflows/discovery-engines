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
#define float32_t float
#define SetBit(A,k)   (A[(k/32)] |=  (1 << (k%32)))
#define ClearBit(A,k) (A[(k/32)] &= ~(1 << (k%32)))
#define TestBit(A,k)  (A[(k/32)] &   (1 << (k%32)))
#define deg2rad 0.0174532925199433
#define rad2deg 57.2957795130823
#define EPS 1E-5
#define MAX_N_SPOTS 200
#define MAX_N_OMEGA_RANGES 20
#define MAX_POINTS_GRID_GOOD 200000

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
Convert9To3x3(
              double MatIn[9],
              double MatOut[3][3])
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
    free(ys);
    free(zs);
    free(peakID);
    free(intensity);
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
    phi = acos(m[2][2]);
    sph = sin(phi);
    if (fabs(sph) < EPS)
    {
        psi = 0.0;
        theta = (fabs(m[2][2] - 1.0) < EPS) ? sin_cos_to_angle(m[1][0], m[0][0]) : sin_cos_to_angle(-m[1][0], m[0][0]);
    } else
    {
        psi = sin_cos_to_angle(m[0][2] / sph, -m[1][2] / sph);
        theta = sin_cos_to_angle(m[2][0] / sph, m[2][1] / sph);
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
MatrixMultF(
    RealType m[3][3],
    RealType v[3],
    RealType r[3])
{
    int i;
    for (i=0; i<3; i++) {
        r[i] = m[i][0]*v[0] + m[i][1]*v[1] + m[i][2]*v[2];
    }
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
    double r3r2[3][3];
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
    MatrixMultF33(r3,r2,r3r2);
    MatrixMultF33(r3r2,r1,RotMatOut);
}

void
RotateAroundZ(
    RealType v1[3],
    RealType alpha,
    RealType v2[3])
{
    RealType cosa = cos(alpha*deg2rad);
    RealType sina = sin(alpha*deg2rad);
    RealType mat[3][3] = {{ cosa, -sina, 0 },
        { sina,  cosa, 0 },
        { 0, 0, 1}};
    MatrixMultF(mat, v1, v2);
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

int sign(x){
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
            }
            else{
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
                *FracOver+=((double)OverlapPixels)/((double)NrInPixels);
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
    CalcDiffractionSpots(LatticeConstant, Wavelength, Lsd0, nRings, ExcludePoleAngle, RingNumbers, OmegaRanges, NoOfOmegaRanges, BoxSizes, &nTspots, OrientMatIn,TheorSpots);
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
    CalcFracOverlap(NrOfFiles,nLayers,nTspots,ThrSps,OmegaStart,OmegaStep,XG,YG,Lsd,SizeObsSpots,RotMatTilts,px,ybc,zbc,gs,P0,NrPixelsGrid,ObsSpotsInfo,OrientMatIn,&FracOver);
    *FracOverlap = FracOver;
    FreeMemMatrix(TheorSpots,MAX_N_SPOTS);
}

int midaco(long int*,long int*,long int*,long int*,long int*,double*,double*,double*,double*,double*,long int*,long int*,double*,double*,long int*,long int*,long int*,char*);
int midaco_print(int,long int,long int,long int*,long int*,double*,double*,double*,double*,double*,long int,long int,long int,long int,double*,long int,long int,double*,long int,char*);

void problem_function(
    double *f,
    double *g,
    double *x,
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
    const int ObsSpotsInfo[SizeObsSpots])
{
    double OrientMatIn[3][3], FracOverlap;
    Euler2OrientMat(x,OrientMatIn);
    CalcOverlapAccOrient(NrOfFiles,nLayers,LatticeConstant,Wavelength,nRings,ExcludePoleAngle,Lsd,SizeObsSpots,XGrain,YGrain,RotMatTilts,OmegaStart,OmegaStep,px,ybc,zbc,gs,RingNumbers,OmegaRanges,NoOfOmegaRanges,BoxSizes,P0,NrPixelsGrid,ObsSpotsInfo,OrientMatIn,&FracOverlap);
    f[0] = 1 - FracOverlap;
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
    double EulerIn[3],
    double tol,
    double *EulerOutA,
    double *EulerOutB,
    double *EulerOutC,
    double *ResultFracOverlap)
{
    long int iw[1000]; double x[100],xl[100],xu[100],g[100],rw[10000],param[9],f[1];
    long int p,n,ni,m,me,maxeval,maxtime,printeval,save2file,i,iflag,istop,liw,lrw;
    char key[] =  "MIDACO_LIMITED_VERSION___[CREATIVE_COMMONS_BY-NC-ND_LICENSE]";
    n  = 3;
    ni = 0;
    m  = 0;
    me = 0;
    for( i=0; i<n; i++)
    {
        x[i] = EulerIn[i];
        xl[i] = x[i] - tol;
        xu[i] = x[i] + tol;
    }
    maxeval = 5000;
    maxtime = 60;
    printeval = 1000000;
    save2file = 0;
    param[0] =  0.0;
    param[1] =  0.0;
    param[2] =  0.0;
    param[3] =  0.0;
    param[4] =  0.0;
    param[5] =  0.0;
    param[6] =  0.0;
    param[7] =  0.0;
    param[8] =  0.0;
    p=1;lrw=sizeof(rw)/sizeof(double);liw=sizeof(iw)/sizeof(long int);
    midaco_print(1,printeval,save2file,&iflag,&istop,&*f,&*g,&*x,&*xl,&*xu,n,ni,m,me,&*rw,maxeval,maxtime,&*param,1,&*key);
    while(istop==0){
        problem_function(&*f,&*g,&*x,NrOfFiles,nLayers,LatticeConstant,Wavelength,nRings,ExcludePoleAngle,Lsd,SizeObsSpots,XGrain,YGrain,RotMatTilts,OmegaStart,OmegaStep,px,ybc,zbc,gs,RingNumbers,OmegaRanges,NoOfOmegaRanges,BoxSizes,P0,NrPixelsGrid,ObsSpotsInfo);
        midaco(&p,&n,&ni,&m,&me,&*x,&*f,&*g,&*xl,&*xu,&iflag,&istop,&*param,&*rw,&lrw,&*iw,&liw,&*key);
        midaco_print(2,printeval,save2file,&iflag,&istop,&*f,&*g,&*x,&*xl,&*xu,n,ni,m,me,&*rw,maxeval,maxtime,&*param,1,&*key);
    }
    problem_function(&*f,&*g,&*x,NrOfFiles,nLayers,LatticeConstant,Wavelength,nRings,ExcludePoleAngle,Lsd,SizeObsSpots,XGrain,YGrain,RotMatTilts,OmegaStart,OmegaStep,px,ybc,zbc,gs,RingNumbers,OmegaRanges,NoOfOmegaRanges,BoxSizes,P0,NrPixelsGrid,ObsSpotsInfo);
    midaco_print(3,printeval,save2file,&iflag,&istop,&*f,&*g,&*x,&*xl,&*xu,n,ni,m,me,&*rw,maxeval,maxtime,&*param,1,&*key);
    *ResultFracOverlap = *f;
    *EulerOutA = x[0];
    *EulerOutB = x[1];
    *EulerOutC = x[2];
}

void
MatrixInv(
    double Matr[3][3],
    double MatrInv[3][3])
{
    double C1;
    C1 = ((Matr[1][1]*Matr[2][2]-Matr[1][2]*Matr[2][1])*Matr[0][0])+((Matr[1][2]*Matr[2][0]-Matr[1][0]*Matr[2][2])*Matr[0][1])+((Matr[1][0]*Matr[2][1]-Matr[1][1]*Matr[2][0])*Matr[0][2]);
    MatrInv[0][0]=(Matr[1][1]*Matr[2][2]-Matr[1][2]*Matr[2][1])/C1;
    MatrInv[1][0]=(Matr[1][2]*Matr[2][0]-Matr[1][0]*Matr[2][2])/C1;
    MatrInv[2][0]=(Matr[1][0]*Matr[2][1]-Matr[1][1]*Matr[2][0])/C1;
    MatrInv[0][1]=(Matr[2][1]*Matr[0][2]-Matr[2][2]*Matr[0][1])/C1;
    MatrInv[1][1]=(Matr[2][2]*Matr[0][0]-Matr[2][0]*Matr[0][2])/C1;
    MatrInv[2][1]=(Matr[2][0]*Matr[0][1]-Matr[2][1]*Matr[0][0])/C1;
    MatrInv[0][2]=(Matr[0][1]*Matr[1][2]-Matr[0][2]*Matr[1][1])/C1;
    MatrInv[1][2]=(Matr[0][2]*Matr[1][0]-Matr[0][0]*Matr[1][2])/C1;
    MatrInv[2][2]=(Matr[0][0]*Matr[1][1]-Matr[0][1]*Matr[1][0])/C1;
}

double SymmetryOperator[24][9] ={
    { 0, 0, 0, 0, 0, 0, 0, 0, 0},//1
    { 0, 1, 0, 0, 0, 1, 1, 0, 0},//2
    { 0, 0, 1, 1, 0, 0, 0, 1, 0},//3
    {-1, 0, 0, 0, 0,-1, 0,-1, 0},//4
    { 0,-1, 0,-1, 0, 0, 0, 0,-1},//5
    { 0, 0,-1, 0,-1, 0,-1, 0, 0},//6
    { 1, 0, 0, 0, 0, 1, 0,-1, 0},//7
    { 0, 0, 1, 0, 1, 0,-1, 0, 0},//8
    { 0, 1, 0, 1, 0, 0, 0, 0,-1},//9
    { 0, 0,-1,-1, 0, 0, 0, 1, 0},//10
    { 0,-1, 0, 0, 0,-1, 1, 0, 0},//11
    {-1, 0, 0, 0,-1, 0, 0, 0, 1},//12
    { 1, 0, 0, 0, 0,-1, 0, 1, 0},//13
    { 0, 0, 1, 0,-1, 0, 1, 0, 0},//14
    { 0, 1, 0,-1, 0, 0, 0, 0, 1},//15
    { 0,-1, 0, 0, 0, 1,-1, 0, 0},//16
    { 0, 0,-1, 1, 0, 0, 0,-1, 0},//17
    {-1, 0, 0, 0, 1, 0, 0, 0,-1},//18
    { 0, 0,-1, 0, 1, 0, 1, 0, 0},//19
    {-1, 0, 0, 0, 0, 1, 0, 1, 0},//20
    { 0,-1, 0, 1, 0, 0, 0, 0, 1},//21
    { 0, 1, 0, 0, 0,-1,-1, 0, 0},//22
    { 0, 0, 1,-1, 0, 0, 0,-1, 0},//23
    { 1, 0, 0, 0,-1, 0, 0, 0,-1},//24
};

void
MinMisOrientationAngle(
    double Orient1[3][3],
    double Orient2[3][3],
    double *MinAngle)
{
    int i,j,k,l;
    double ANS[3][3], Sym[3][3], thet, AngTmp, MisOrientation[3][3],Orient1Inv[3][3];
    MatrixInv(Orient1,Orient1Inv);
    MatrixMultF33(Orient2,Orient1Inv,MisOrientation);
    *MinAngle = 100000;
    for (i=0;i<24;i++){
        l=0;
        for (j=0;j<3;j++){
            for (k=0;k<3;k++){
                Sym[j][k] = SymmetryOperator[i][l];
                l++;
            }
        }
        MatrixMultF33(Sym,MisOrientation,ANS);
        thet = ANS[0][0] + ANS[1][1] + ANS[2][2];
        if (thet < -1){
            thet = -1;
        }
        else if (thet > 2){
            thet = 2;
        }
        AngTmp = fabs(rad2deg*acos((thet-1)/2));
        if (AngTmp < *MinAngle){
            *MinAngle = AngTmp;
        }
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
        return EXIT_FAILURE;
    }
    
    //Read position.
    char *fnG="grid.txt";
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
        CalcFracOverlap(nrFiles,nLayers,NrSpotsThis,ThrSps,OmegaStart,OmegaStep,XG,YG,Lsd,SizeObsSpots,RotMatTilts,px,ybc,zbc,gs,P0,NrPixelsGrid,ObsSpotsInfo,OrientMatIn,&FracOverT);
        if (FracOverT >= minFracOverlap){
            for (j=0;j<9;j++){
                OrientMatrix[OrientationGoodID][j] = OrientationMatThis[j];
            }
            OrientMatrix[OrientationGoodID][9] = FracOverT;
            OrientationGoodID++;
        }
    }
    if (OrientationGoodID>0){
        double Fractions[OrientationGoodID], EulerAnglesFit[OrientationGoodID][3],EulerIn[3],OrientIn[3][3], FracOut, EulerOutA, EulerOutB,EulerOutC,BestFrac,BestEuler[3],XG[3],YG[3],OMTemp[9];
        BestFrac = -1;
        for (i=0;i<OrientationGoodID;i++){
            for (j=0;j<9;j++){
                OMTemp[j] = OrientMatrix[i][j];
            }
            Convert9To3x3(OMTemp,OrientIn);
            OrientMat2Euler(OrientIn,EulerIn);
            FitOrientation(nrFiles,nLayers,LatticeConstant,Wavelength,nRings,ExcludePoleAngle,Lsd,SizeObsSpots,XG,YG,RotMatTilts,OmegaStart,OmegaStep,px,ybc,zbc,gs,RingNumbers,OmegaRanges,NoOfOmegaRanges,BoxSizes,P0,NrPixelsGrid,ObsSpotsInfo,EulerIn,tol,&EulerOutA,&EulerOutB,&EulerOutC,&FracOut);
            EulerAnglesFit[i][0] = EulerOutA;
            EulerAnglesFit[i][1] = EulerOutB;
            EulerAnglesFit[i][2] = EulerOutC;
            Fractions[i] = 1-FracOut;
            if (Fractions[i] > BestFrac){
                BestFrac = Fractions[i];
                for (j=0;j<3;j++){
                    BestEuler[j] = EulerAnglesFit[i][j];
                }
            }
        }
        printf("%f %f %f %f\n",BestEuler[0],BestEuler[1],BestEuler[2],BestFrac);
    }
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
