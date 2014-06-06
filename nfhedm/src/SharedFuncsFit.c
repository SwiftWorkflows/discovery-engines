
#include <math.h>
#include <time.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <nlopt.h>
#include <arpa/inet.h>

#include "checks.h"
#include "Debug.h"
#include "SharedFuncsFit.h"
#include "CalcDiffractionSpots.h"
#include "FitOrientation.h"

#define RealType double
#define SetBit(A,k)   (A[(k/32)] |=  (1 << (k%32)))
#define ClearBit(A,k) (A[(k/32)] &= ~(1 << (k%32)))
#define TestBit(A,k)  (A[(k/32)] &   (1 << (k%32)))
#define EPS 1E-5
#define MAX_N_SPOTS 200

double**
allocMatrix(int nrows, int ncols)
{
    double** arr;
    int i;

    // printf("allocMatrix: %ix%i\n", nrows, ncols);
    
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

bool ReadHeader(
    FILE *fp,
    struct Theader * head)
{
    // printf("ReadHeader\n");
    READ(&head->uBlockHeader,sizeof(uint32_t),1,fp);
    READ(&head->BlockType,sizeof(uint16_t),1,fp);
    READ(&head->DataFormat,sizeof(uint16_t),1,fp);
    READ(&head->NumChildren,sizeof(uint16_t),1,fp);
    READ(&head->NameSize,sizeof(uint16_t),1,fp);
    READ(&head->DataSize,sizeof(uint32_t),1,fp);
    READ(&head->ChunkNumber,sizeof(uint16_t),1,fp);
    READ(&head->TotalChunks,sizeof(uint16_t),1,fp);
    READ(&head->BlockName,(sizeof(char)*(head->NameSize)),1,fp);
    head->BlockName[head->NameSize] = '\0';
    return true;
}

#define append(string,  args...) \
  string += sprintf(string, ## args)

void WriteHeader(
    FILE *fp,
    struct Theader * head)
{
    fwrite(&head->uBlockHeader,sizeof(uint32_t),1,fp);
    fwrite(&head->BlockType,sizeof(uint16_t),1,fp);
    fwrite(&head->DataFormat,sizeof(uint16_t),1,fp);
    fwrite(&head->NumChildren,sizeof(uint16_t),1,fp);
    fwrite(&head->NameSize,sizeof(uint16_t),1,fp);
    fwrite(&head->DataSize,sizeof(uint32_t),1,fp);
    fwrite(&head->ChunkNumber,sizeof(uint16_t),1,fp);
    fwrite(&head->TotalChunks,sizeof(uint16_t),1,fp);
    fwrite(&head->BlockName,(sizeof(char)*(head->NameSize)),1,fp);
}

void NWriteHeader(
    FILE *fp,
    struct Theader * head)
{
    uint16_t t16;
    uint32_t t32;
    t32 = htonl(head->uBlockHeader);
    fwrite(&t32,sizeof(uint32_t),1,fp);
    t16 = htons(head->BlockType);
    fwrite(&t16,sizeof(uint16_t),1,fp);
    t16 = htons(head->DataFormat);
    fwrite(&t16,sizeof(uint16_t),1,fp);
    t16 = htons(head->NumChildren);
    fwrite(&t16,sizeof(uint16_t),1,fp);
    t16 = htons(head->NameSize);
    fwrite(&t16,sizeof(uint16_t),1,fp);
    t32 = htonl(head->DataSize);
    fwrite(&t32,sizeof(uint32_t),1,fp);
    t16 = htons(head->ChunkNumber);
    fwrite(&t16,sizeof(uint16_t),1,fp);
    t16 = htons(head->TotalChunks);
    fwrite(&t16,sizeof(uint16_t),1,fp);
    fwrite(&head->BlockName,(sizeof(char)*(head->NameSize)),1,fp);
}

void PrintHeader(
    FILE *fp,
    struct Theader * head)
{
    fprintf(fp, "BlockHeader: %"PRIu32"\n",  head->uBlockHeader);
    fprintf(fp, "BlockType:   %"PRIu16"\n" , head->BlockType);
    fprintf(fp, "DataFormat:  %"PRIu16"\n" , head->DataFormat);
    fprintf(fp, "NumChildren: %"PRIu16"\n" , head->NumChildren);
    fprintf(fp, "NameSize:    %"PRIu16"\n" , head->NameSize);
    fprintf(fp, "DataSize:    %"PRIu32"\n" , head->DataSize);
    fprintf(fp, "ChunkNumber: %"PRIu16"\n" , head->ChunkNumber);
    fprintf(fp, "TotalChunks: %"PRIu16"\n" , head->TotalChunks);
    fprintf(fp, "BlockName:   '%s'\n",       head->BlockName);
}

void PrintUint32s(
        FILE *fp,
        uint32_t *data,
        int count)
{
    int bytes = count*24; // Up to 24 characters per line
    assert(bytes < 1024*1024);
    char s[bytes];
    char *p = &s[0];
    char *q = p;
    for (int i = 0; i < count; i++)
        append(q, "%i: %"PRIu32"\n", i, data[i]);
    *q = '\0';
    fwrite(p, sizeof(char), q-p, fp);
}

void PrintUint16s(
        FILE *fp,
        uint16_t *data,
        int count)
{
    int bytes = count*16; // Up to 16 characters per line
    assert(bytes < 1024*1024);
    char s[bytes];
    char *p = &s[0];
    char *q = p;
    for (int i = 0; i < count; i++)
        append(q, "%i: %"PRIu16"\n", i, data[i]);
    fwrite(p, sizeof(char), q-p, fp);
}

void PrintFloat32s(
        FILE *fp,
        float32_t *data,
        int count)
{
    int bytes = count*32; // Up to 32 characters per line
    assert(bytes < 1024*1024);
    char s[bytes];
    char *p = &s[0];
    char *q = p;
    for (int i = 0; i < count; i++)
        append(q, "%i: %0.3f\n", i, (double) data[i]);
    fwrite(p, sizeof(char), q-p, fp);
}

bool
ReadBinFiles(
    char FileStem[1000],
    char *ext,
    int StartNr,
    int EndNr,
    int *ObsSpotsMat,
    int nLayers,
    long long int ObsSpotsSize)
{
    int i,j,k,nElements=0,nElements_previous,nCheck,ythis,zthis,NrOfFiles,NrOfPixels;
    long long int BinNr;
    long long int TempCntr;
    float32_t dummy;
    uint32_t dummy2;
    FILE *fp;
    char FileName[1024];
    struct Theader Header1;
    uint16_t *ys=NULL, *zs=NULL, *peakID=NULL;
    float32_t *intensity=NULL;
    int counter=0;

    LOG("ReadBinFiles: Start: %i End: %i Total: %i",
           StartNr, EndNr, EndNr-StartNr+1);
    PROFILE_CREATE(ReadBinFiles, p);
    PROFILE_START(p);
    NrOfFiles = EndNr - StartNr + 1;
    NrOfPixels = 2048*2048;
    for (k=0;k<ObsSpotsSize;k++){
        ObsSpotsMat[k] = 0;
    }
    for (k=0;k<nLayers;k++){
        for (i=StartNr;i<=EndNr;i++){
            sprintf(FileName,"%s_%06d.%s%d",FileStem,i,ext,k);
            DEBUG("Reading file : %s\n",FileName);
            fp = fopen(FileName,"r");
            if (fp == NULL)
                file_not_found(FileName);
            READ(&dummy,sizeof(float32_t),1,fp);
            ReadHeader(fp,&Header1);
            READ(&dummy2,sizeof(uint32_t),1,fp);
            READ(&dummy2,sizeof(uint32_t),1,fp);
            READ(&dummy2,sizeof(uint32_t),1,fp);
            READ(&dummy2,sizeof(uint32_t),1,fp);
            READ(&dummy2,sizeof(uint32_t),1,fp);
            ReadHeader(fp,&Header1);
            nElements_previous = nElements;
            nElements = (Header1.DataSize - Header1.NameSize)/2;
            realloc_buffers(nElements,nElements_previous,&ys,&zs,&peakID,&intensity);
            /*ys = malloc(nElements*sizeof(*ys));
            zs = malloc(nElements*sizeof(*zs));
            peakID = malloc(nElements*sizeof(*peakID));
            intensity = malloc(nElements*sizeof(*intensity));*/
            READ(ys,sizeof(uint16_t)*nElements,1,fp);
            ReadHeader(fp,&Header1);
            nCheck = (Header1.DataSize - Header1.NameSize)/2;
            if (nCheck!=nElements){
                printf("Number of elements mismatch. (1)\n");
                return false;
            }
            READ(zs,sizeof(uint16_t)*nElements,1,fp);
            ReadHeader(fp,&Header1);
            nCheck = (Header1.DataSize - Header1.NameSize)/4;
            if (nCheck!=nElements){
                printf("Number of elements mismatch. (2)\n");
                return false;
            }
            READ(intensity,sizeof(float32_t)*nElements,1,fp);
            ReadHeader(fp,&Header1);
            nCheck = (Header1.DataSize - Header1.NameSize)/2;
            if (nCheck!=nElements){
                printf("Number of elements mismatch. (3)\n");
                return false;
            }
            READ(peakID,sizeof(uint16_t)*nElements,1,fp);
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
            DEBUG("Closed file : %s\n",FileName);
            counter+=1;
        }
        counter = 0;
    }
    PROFILE_END(p);
    LOG("ReadBinFiles: done.");
    // TODO: Check use of free() here:

//    free(ys);
//    free(zs);
//    free(peakID);
//    free(intensity);
    return true;
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
    } else{
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
    RealType Lsd,
    RealType yi,
    RealType zi,
    RealType omega,
    RealType *Displ_y,
    RealType *Displ_z)
{
    RealType OmegaRad = deg2rad * omega;
    RealType sinOme = sin(OmegaRad);
    RealType cosOme = cos(OmegaRad);
    RealType xa = a*cosOme - b*sinOme;
    RealType ya = a*sinOme + b*cosOme;
    RealType t = 1 - (xa/Lsd);
    *Displ_y = ya + (yi*t);
    *Displ_z = t*zi;
}

static inline double len2d(struct Point2D a, struct Point2D b, struct Point2D c)
{
    return fabs(((b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x)))/sqrt(((a.y-b.y)*(a.y-b.y))+((b.x-a.x)*(b.x-a.x)));
}

static inline void
CalcPixels2(double Edges[3][2], int m, int *Pixels, int *counter)
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
    int c = 0;
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
                Pixels[2*c+0] = p.x;
                Pixels[2*c+1] = p.y;
                c+=1;
            }
            else if(len2d(v1,v2,p)<0.99){
                Pixels[2*c+0] = p.x;
                Pixels[2*c+1] = p.y;
                c+=1;
            }
            else if(len2d(v2,v0,p)<0.99){
                Pixels[2*c+0] = p.x;
                Pixels[2*c+1] = p.y;
                c+=1;
            }
            else if(len2d(v0,v1,p)<0.99){
                Pixels[2*c+0] = p.x;
                Pixels[2*c+1] = p.y;
                c+=1;
            }
            w0 += A12;
            w1 += A20;
            w2 += A01;
        }
        w0_row += B12;
        w1_row += B20;
        w2_row += B01;
    }
    *counter = c;
}

void 
PrintVector(const char* name, const double* v, int count)
{
  for (int i = 0; i < count; i++)
    printf("%s[%i]=%f\n", name, i, v[i]);
}

void
PrintMatrixLinear(const char* name, const double* A, int m, int n)
{
  for (int i = 0; i < m; i++)
      for (int j = 0; j < n; j++)
      {
          printf("%s[%i][%i]", name, i, j);
          fflush(stdout);
          printf("=%0.4f\n", A[i*m+j]);
          fflush(stdout);
      }
}

void
PrintMatrix(const char* name, int m, int n, const double A[m][n])
{
  for (int i = 0; i < m; i++)
      for (int j = 0; j < n; j++)
      {
          printf("%s[%i][%i]", name, i, j);
          fflush(stdout);
          printf("=%0.4f\n", A[i][j]);
          fflush(stdout);
      }
}

void gsl_vector_print(const char *name, gsl_vector *v)
{
    for (int i = 0; i < 3; i++)
        printf("%s[%i]: %0.3f\n", name, i, gsl_vector_get(v, i));
}

void gsl_matrix_print(const char *name, gsl_matrix *A)
{
    for (int i = 0; i < A->size1; i++)
        for (int j = 0; j < A->size2; j++)
        {
            printf("%s[%i][%i]: ", name, i, j);
            fflush(stdout);
            printf("%0.3f\n", gsl_matrix_get(A, i, j));
            fflush(stdout);
        }
}

// #define PRINT_VECTOR()

void
CalcFracOverlap(
    const int NrOfFiles,
    const int nLayers,
    const int nTspots,
    // double TheorSpots[MAX_N_SPOTS][3],
    gsl_matrix *TheorSpots,
    double OmegaStart,
    double OmegaStep,
    double XGrain[3],
    double YGrain[3],
    const double Lsds[nLayers],
    const long long int SizeObsSpots,
    // double RotMatTilts[3][3],
    gsl_matrix *RotMatTilts,
    const double px,
    const double ybcs[nLayers],
    const double zbcs[nLayers],
    const double gs,
    double P0All[nLayers][3],
    const int NrPixelsGrid,
    const int ObsSpotsInfo[SizeObsSpots],
    double OrientMatIn[3][3],
    /*OUT*/ double *FracOver)
{
  PROFILE_START(profile_calc_frac_overlap);
  static bool printed = false;
  if (! printed)
  {
    printf("CalcFracOverlap: NrOfFiles=%i nLayers=%i nTspots=%i "
           "TheorSpots OmegaStart=%f OmegaStop=%f\n",
           NrOfFiles, nLayers, nTspots, OmegaStart, OmegaStep);
    PrintVector("XGrain", XGrain, 3);
    PrintVector("YGrain", YGrain, 3);
    PrintVector("Lsds", Lsds, nLayers);
    printed = true;
  }

  int j,OmeBin,OutofBounds,k,l;
  double OmegaThis,ythis,zthis,XGT,YGT,Displ_Y,Displ_Z,ytemp,ztemp,
  // xyz[3],
  // P1[3],
  ABC[3],outxyz[3],YZSpots[3][2],Lsd,ybc,zbc,P0[3],
  YZSpotsTemp[2],YZSpotsT[3][2];
  double xyzData[3];
  gsl_vector_view xyzView = gsl_vector_view_array(xyzData, 3);
  gsl_vector *xyz = &xyzView.vector;
  double P1Data[3] = { 0.0, 0.0, 0.0 };
  gsl_vector_view P1View = gsl_vector_view_array(P1Data, 3);
  gsl_vector *P1 = &P1View.vector;
  YZSpotsTemp[0] = 0.0;
  YZSpotsTemp[1] = 0.0;
  // int **InPixels;
  int NrInPixels, OverlapPixels,Layer;
  long long int BinNr;
  int MultY, MultZ, AllDistsFound, TotalPixels;
  *FracOver = 0;
  // InPixels = allocMatrixInt(NrPixelsGrid,2);
  int InPixels[NrPixelsGrid*2];

  OverlapPixels = 0;
  TotalPixels=0;
  for (j=0;j<nTspots;j++){
      // ythis = TheorSpots[j][0];
      // zthis = TheorSpots[j][1];
      // OmegaThis = TheorSpots[j][2];
      ythis = gsl_matrix_get(TheorSpots, j, 0);
      zthis = gsl_matrix_get(TheorSpots, j, 1);
      OmegaThis = gsl_matrix_get(TheorSpots, j, 2);
      OmeBin = (int) floor((-OmegaStart+OmegaThis)/OmegaStep);
      for (k=0;k<3;k++){
          P0[k] = P0All[0][k];
      }
      Lsd = Lsds[0];
      ybc = ybcs[0];
      zbc = zbcs[0];
      OutofBounds = 0;
      for (k=0;k<3;k++){
          XGT = XGrain[k];
          YGT = YGrain[k];
          DisplacementSpots(XGT,YGT,Lsd,ythis,zthis,OmegaThis,&Displ_Y,&Displ_Z);
          ytemp = Displ_Y;
          ztemp = Displ_Z;
          //          xyz[0] = 0;
          //          xyz[1] = ytemp;
          //          xyz[2] = ztemp;
          gsl_vector_set(xyz, 0, 0);
          gsl_vector_set(xyz, 1, ytemp);
          gsl_vector_set(xyz, 2, ztemp);
          // MatrixMultF(RotMatTilts,xyz,P1);
          // gsl_vector_print("xyz", xyz);
          // gsl_matrix_print("RotMatTilts", RotMatTilts);
          nfhedm_mvm(RotMatTilts,xyz,P1);
          // printf("dgemv ok.\n");
          // gsl_vector_print("P1", P1);
          for (l=0;l<3;l++){
              // ABC[l] = P1[l]-P0[l];
              double P1l = gsl_vector_get(P1, l);
              ABC[l] = P1l-P0[l];
          }
          outxyz[0] = 0;
          outxyz[1] = P0[1] -(ABC[1]*P0[0])/(ABC[0]);
          outxyz[2] = P0[2] -(ABC[2]*P0[0])/(ABC[0]);
          YZSpotsT[k][0] = (outxyz[1])/px + ybc;
          YZSpotsT[k][1] = (outxyz[2])/px + zbc;
          if (YZSpotsT[k][0] > 2048 || YZSpotsT[k][0] < 0 || YZSpotsT[k][1] > 2048 || YZSpotsT[k][1] < 0){
              OutofBounds = 1;
              break;
          }
          if (k==2){
              //              xyz[0] = 0;
              //              xyz[1] = ythis;
              //              xyz[2] = zthis;
              gsl_vector_set(xyz, 0, 0);
              gsl_vector_set(xyz, 1, ythis);
              gsl_vector_set(xyz, 2, zthis);
              // MatrixMultF(RotMatTilts,xyz,P1);
              nfhedm_mvm(RotMatTilts,xyz,P1);
              for (l=0;l<3;l++){
                  // ABC[l] = P1[l]-P0[l];
                  double P1l = gsl_vector_get(P1, l);
                  ABC[l] = P1l-P0[l];
              }
              outxyz[0] = 0;
              outxyz[1] = P0[1] -(ABC[1]*P0[0])/(ABC[0]);
              outxyz[2] = P0[2] -(ABC[2]*P0[0])/(ABC[0]);
              YZSpotsTemp[0] = (outxyz[1])/px + ybc;
              YZSpotsTemp[1] = (outxyz[2])/px + zbc;
              for (l=0;l<3;l++){
                  YZSpots[l][0] = YZSpotsT[l][0] - YZSpotsTemp[0];
                  YZSpots[l][1] = YZSpotsT[l][1] - YZSpotsTemp[1];
                  //printf("%f %f %f %f %f %f\n",YZSpots[l][0],YZSpotsT[l][0],YZSpotsTemp[0],YZSpots[l][1],YZSpotsT[l][1],YZSpotsTemp[1]);
              }
          }
      }
      if (OutofBounds == 1){
          continue;
      }
      if (gs*2 > px){
          CalcPixels2(YZSpots, NrPixelsGrid, &InPixels[0],&NrInPixels);
      }else{
          InPixels[0] = (int) round((YZSpots[0][0]+YZSpots[1][0]+YZSpots[2][0])/3);
          InPixels[1] = (int) round((YZSpots[0][1]+YZSpots[1][1]+YZSpots[2][1])/3);
          NrInPixels = 1;
      }
      for (k=0;k<NrInPixels;k++){
          AllDistsFound = 1;
          for (Layer=0;Layer<nLayers;Layer++){
              MultY = (int) floor(((((double)(YZSpotsTemp[0]-ybc))*px)*(Lsds[Layer]/Lsd))/px + ybcs[Layer]) + InPixels[2*k+0];
              MultZ = (int) floor(((((double)(YZSpotsTemp[1]-zbc))*px)*(Lsds[Layer]/Lsd))/px + zbcs[Layer]) + InPixels[2*k+1];
              if (MultY > 2048 || MultY < 0 || MultZ > 2048 || MultZ < 0){
                  OutofBounds = 1;
                  break;
              }
              BinNr = Layer*NrOfFiles;
              BinNr *= 2048;
              BinNr *= 2048;
              BinNr += OmeBin*2048*2048;
              BinNr += 2048*MultY;
              BinNr += MultZ;
              if (TestBit(ObsSpotsInfo,BinNr)){
                  if (AllDistsFound == 1){
                      AllDistsFound = 1;
                  }
              }else{
                  AllDistsFound = 0;
              }
          }
          if (OutofBounds == 1){
              continue;
          }
          if (AllDistsFound == 1){
              OverlapPixels += 1;
          }
          TotalPixels+=1;
      }
  }
  if (TotalPixels > 0){
      *FracOver = (double)((double)OverlapPixels)/((double)TotalPixels);
  }
  PROFILE_END(profile_calc_frac_overlap);
  // FreeMemMatrixInt(InPixels,NrPixelsGrid);
}

void
CalcOverlapAccOrient(
    const int NrOfFiles,
    const int nLayers,
    const double ExcludePoleAngle,
    const double Lsd[nLayers],
    const long long int SizeObsSpots,
    const double XGrain[3],
    const double YGrain[3],
    // double RotMatTilts[3][3],
    gsl_matrix *RotMatTilts,
    const double OmegaStart,
    const double OmegaStep,
    const double px,
    const double ybc[nLayers],
    const double zbc[nLayers],
    const double gs,
    int hkls[5000][4],
    int n_hkls,
    double Thetas[5000],
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
    CalcDiffractionSpots(Lsd0, ExcludePoleAngle, OmegaRanges, NoOfOmegaRanges,
		hkls, n_hkls, Thetas, BoxSizes, &nTspots, OrientMatIn,TheorSpots);
    double FracOver;
    double XG[3],YG[3];
    // double ThrSps[nTspots][3];
    double ThrSpsData[nTspots*3];
    gsl_matrix_view ThrSpsView = gsl_matrix_view_array(ThrSpsData, nTspots, 3);
    gsl_matrix *ThrSps = &ThrSpsView.matrix;
    for (i=0;i<3;i++){
        XG[i] = XGrain[i];
        YG[i] = YGrain[i];
    }
    for (i=0;i<nTspots;i++){
        gsl_matrix_set(ThrSps, i, 0, TheorSpots[i][0]);
        gsl_matrix_set(ThrSps, i, 1, TheorSpots[i][1]);
        gsl_matrix_set(ThrSps, i, 2, TheorSpots[i][2]);
    }
    CalcFracOverlap(NrOfFiles,nLayers,nTspots,ThrSps,OmegaStart,
		OmegaStep,XG,YG,Lsd,SizeObsSpots,RotMatTilts,px,ybc,zbc,gs,
		P0,NrPixelsGrid,ObsSpotsInfo,OrientMatIn,&FracOver);
    *FracOverlap = FracOver;
    FreeMemMatrix(TheorSpots,MAX_N_SPOTS);
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
hton_Uint16s(uint16_t  *data, int count) {
    for (int i = 0; i < count; i++)
        data[i] = htons(data[i]);
}
void
hton_Uint32s(uint32_t  *data, int count) {
    for (int i = 0; i < count; i++)
        data[i] = htonl(data[i]);
}

void
hton_Float32s(float32_t *data, int count) {
    int32_t t;
    for (int i = 0; i < count; i++) {
        // These memcpy's are required for correctness
        memcpy(&t, &data[i], 4);
        int32_t v = htonl(t);
        memcpy(&data[i], &v, 4);
    }
}
