//
//  CalcDiffractionSpots.c
//  
//
//  Created by Hemant Sharma on 12/3/13.
//
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "CalcDiffractionSpots.h"

#define deg2rad 0.0174532925199433
#define rad2deg 57.2957795130823
#define RealType double
#define MAX_N_HKLS 1000

#define crossProduct(a,b,c) \
(a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; \
(a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; \
(a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];

#define dot(v,q) \
((v)[0] * (q)[0] + \
(v)[1] * (q)[1] + \
(v)[2] * (q)[2])

#define CalcLength(x,y,z) sqrt((x)*(x) + (y)*(y) + (z)*(z))

int n_hkls = 0;
int hkls[MAX_N_HKLS][4];


void
MatrixMult(
           RealType m[3][3],
           int  v[3],
           RealType r[3])
{
    int i;
    for (i=0; i<3; i++) {
        r[i] = m[i][0]*v[0] +
        m[i][1]*v[1] +
        m[i][2]*v[2];
    }
}

void
GenerateHKLsFCC(
                const int ringnos[],
                int nrings,
                int hkls[][4],
                int *nhkls)

{
    
#define N_HKLSFCC 58
    int hklsFCC[N_HKLSFCC][4] ={
        {-1,-1,-1,1},
        { 1, 1, 1,1},
        {-1,-1, 1,1},
        {-1, 1,-1,1},
        {-1, 1, 1,1},
        { 1,-1,-1,1},
        { 1,-1, 1,1},
        { 1, 1,-1,1},
        
        {-2, 0, 0,2},
        { 0,-2, 0,2},
        { 0, 0,-2,2},
        { 0, 0, 2,2},
        { 0, 2, 0,2},
        { 2, 0, 0,2},
        
        {-2,-2, 0,3},
        {-2, 0,-2,3},
        {-2, 0, 2,3},
        {-2, 2, 0,3},
        { 0,-2,-2,3},
        { 0,-2, 2,3},
        { 0, 2,-2,3},
        { 0, 2, 2,3},
        { 2,-2, 0,3},
        { 2, 0,-2,3},
        { 2, 0, 2,3},
        { 2, 2, 0,3},
        
        {-3,-1,-1,4},
        {-1,-3,-1,4},
        { 1, 3, 1,4},
        { 3, 1, 1,4},
        {-3,-1, 1,4},
        {-3, 1,-1,4},
        {-3, 1, 1,4},
        {-1,-3, 1,4},
        {-1,-1,-3,4},
        {-1,-1, 3,4},
        {-1, 1,-3,4},
        {-1, 1, 3,4},
        {-1, 3,-1,4},
        {-1, 3, 1,4},
        { 1,-3,-1,4},
        { 1,-3, 1,4},
        { 1,-1,-3,4},
        { 1,-1, 3,4},
        { 1, 1,-3,4},
        { 1, 1, 3,4},
        { 1, 3,-1,4},
        { 3,-1,-1,4},
        { 3,-1, 1,4},
        { 3, 1,-1,4},
        
        {-2,-2,-2,5},
        { 2, 2, 2,5},
        {-2,-2, 2,5},
        {-2, 2,-2,5},
        {-2, 2, 2,5},
        { 2,-2,-2,5},
        { 2,-2, 2,5},
        { 2, 2,-2,5} };
    
    int i, ringno, rowno;
    
    *nhkls = 0;
    for ( i = 0 ; i < nrings ; i++ ) {
        ringno = ringnos[i];
        for ( rowno = 0 ; rowno < N_HKLSFCC ; rowno++ ) {
            if (hklsFCC[rowno][3] == ringno) {
                hkls[*nhkls][0] = hklsFCC[rowno][0];
                hkls[*nhkls][1] = hklsFCC[rowno][1];
                hkls[*nhkls][2] = hklsFCC[rowno][2];
                hkls[*nhkls][3] = hklsFCC[rowno][3];
                (*nhkls)++;
            }
        }
    }
}

void
QuatToOrientMat(
                double Quat[4],
                double OrientMat[3][3])
{
    double Q1_2,Q2_2,Q3_2,Q12,Q03,Q13,Q02,Q23,Q01;
    Q1_2 = Quat[1]*Quat[1];
    Q2_2 = Quat[2]*Quat[2];
    Q3_2 = Quat[3]*Quat[3];
    Q12  = Quat[1]*Quat[2];
    Q03  = Quat[0]*Quat[3];
    Q13  = Quat[1]*Quat[3];
    Q02  = Quat[0]*Quat[2];
    Q23  = Quat[2]*Quat[3];
    Q01  = Quat[0]*Quat[1];
    OrientMat[0][0] = 1 - 2*(Q2_2+Q3_2);
    OrientMat[0][1] = 2*(Q12+Q03);
    OrientMat[0][2] = 2*(Q13-Q02);
    OrientMat[1][0] = 2*(Q12-Q03);
    OrientMat[1][1] = 1 - 2*(Q1_2+Q3_2);
    OrientMat[1][2] = 2*(Q23+Q01);
    OrientMat[2][0] = 2*(Q13+Q02);
    OrientMat[2][1] = 2*(Q23-Q01);
    OrientMat[2][2] = 1 - 2*(Q1_2+Q2_2);
}

void
CalcTheta(
          int h,
          int k,
          int l,
          RealType LatticeParameter,
          RealType Wavelength,
          RealType *theta)
{
    RealType dspacing;
    
    RealType h2k2l2 = h*h + k*k + l*l;
    dspacing = sqrt(LatticeParameter * LatticeParameter/h2k2l2);
    *theta = rad2deg*asin(Wavelength/(2*dspacing));
}

void
GenerateRingInfo(
                 RealType LatticeConstant,
                 RealType Wavelength,
                 RealType RingTtheta[],
                 int   RingMult[],
                 int   RingHKL[][3])

{
    RealType theta;
    RingTtheta[0] = 0;
    CalcTheta(1, 1, 1, LatticeConstant, Wavelength, &theta);
    RingTtheta[1] = 2*theta;
    CalcTheta(2, 0, 0, LatticeConstant, Wavelength, &theta);
    RingTtheta[2] = 2*theta;
    CalcTheta(2, 2, 0, LatticeConstant, Wavelength, &theta);
    RingTtheta[3] = 2*theta;
    CalcTheta(3, 1, 1, LatticeConstant, Wavelength, &theta);
    RingTtheta[4] = 2*theta;
    CalcTheta(2, 2, 2, LatticeConstant, Wavelength, &theta);
    RingTtheta[5] = 2*theta;
    
    RingMult[0] = 0;
    RingMult[1] = 8;
    RingMult[2] = 6;
    RingMult[3] = 12;
    RingMult[4] = 24;
    RingMult[5] = 8;
    
    RingHKL[0][0] = 0;
    RingHKL[0][1] = 0;
    RingHKL[0][2] = 0;
    
    RingHKL[1][0] = 1;
    RingHKL[1][1] = 1;
    RingHKL[1][2] = 1;
    
    RingHKL[2][0] = 2;
    RingHKL[2][1] = 0;
    RingHKL[2][2] = 0;
    
    RingHKL[3][0] = 2;
    RingHKL[3][1] = 2;
    RingHKL[3][2] = 0;
    
    RingHKL[4][0] = 3;
    RingHKL[4][1] = 1;
    RingHKL[4][2] = 1;
    
    RingHKL[5][0] = 2;
    RingHKL[5][1] = 2;
    RingHKL[5][2] = 2;
    
}

void
CalcEtaAngle(
             RealType y,
             RealType z,
             RealType *alpha) {
    *alpha = rad2deg * acos(z/sqrt(y*y+z*z));
    if (y > 0)    *alpha = -*alpha;
}

void
CalcSpotPosition(
                 RealType RingRadius,
                 RealType eta,
                 RealType *yl,
                 RealType *zl)
{
    RealType etaRad = deg2rad * eta;
    *yl = -(sin(etaRad)*RingRadius);
    *zl =   cos(etaRad)*RingRadius;
}

void
CalcOmega(
          RealType x,
          RealType y,
          RealType z,
          RealType theta,
          RealType omegas[4],
          RealType etas[4],
          int * nsol)
{
    *nsol = 0;
    RealType ome;
    RealType len= sqrt(x*x + y*y + z*z);
    RealType v=sin(theta*deg2rad)*len;
    
    RealType almostzero = 1e-4;
    if ( fabs(y) < almostzero ) {
        if (x != 0) {
            RealType cosome1 = -v/x;
            if (fabs(cosome1 <= 1)) {
                ome = acos(cosome1)*rad2deg;
                omegas[*nsol] = ome;
                *nsol = *nsol + 1;
                omegas[*nsol] = -ome;
                *nsol = *nsol + 1;
            }
        }
    }
    else {
        RealType y2 = y*y;
        RealType a = 1 + ((x*x) / y2);
        RealType b = (2*v*x) / y2;
        RealType c = ((v*v) / y2) - 1;
        RealType discr = b*b - 4*a*c;
        
        RealType ome1a;
        RealType ome1b;
        RealType ome2a;
        RealType ome2b;
        RealType cosome1;
        RealType cosome2;
        
        RealType eqa, eqb, diffa, diffb;
        
        if (discr >= 0) {
            cosome1 = (-b + sqrt(discr))/(2*a);
            if (fabs(cosome1) <= 1) {
                ome1a = acos(cosome1);
                ome1b = -ome1a;
                eqa = -x*cos(ome1a) + y*sin(ome1a);
                diffa = fabs(eqa - v);
                eqb = -x*cos(ome1b) + y*sin(ome1b);
                diffb = fabs(eqb - v);
                if (diffa < diffb ) {
                    omegas[*nsol] = ome1a*rad2deg;
                    *nsol = *nsol + 1;
                }
                else {
                    omegas[*nsol] = ome1b*rad2deg;
                    *nsol = *nsol + 1;
                }
            }
            
            cosome2 = (-b - sqrt(discr))/(2*a);
            if (fabs(cosome2) <= 1) {
                ome2a = acos(cosome2);
                ome2b = -ome2a;
                
                eqa = -x*cos(ome2a) + y*sin(ome2a);
                diffa = fabs(eqa - v);
                eqb = -x*cos(ome2b) + y*sin(ome2b);
                diffb = fabs(eqb - v);
                
                if (diffa < diffb) {
                    omegas[*nsol] = ome2a*rad2deg;
                    *nsol = *nsol + 1;
                }
                else {
                    omegas[*nsol] = ome2b*rad2deg;
                    *nsol = *nsol + 1;
                }
            }
        }
    }
    RealType gw[3];
    RealType gv[3]={x,y,z};
    RealType eta;
    int indexOme;
    for (indexOme = 0; indexOme < *nsol; indexOme++) {
        RotateAroundZ(gv, omegas[indexOme], gw);
        CalcEtaAngle(gw[1],gw[2], &eta);
        etas[indexOme] = eta;
    }
}

void
CalcRingRadii(
              RealType Distance,
              RealType Ttheta[],
              const int RingNrs[],
              int NrRings,
              RealType RingRadii[])
{
    int i;
    for (i=0;i<NrRings;i++){
        RingRadii[i+1] = tan(deg2rad*Ttheta[RingNrs[i]])*Distance;
    }
}

void
CalcDiffrSpots_Furnace(
                       RealType OrientMatrix[3][3],
                       RealType LatticeConstant,
                       RealType Wavelength ,
                       RealType distance,
                       RealType RingRadii[],
                       RealType OmegaRange[][2],
                       RealType BoxSizes[][4],
                       int NOmegaRanges,
                       RealType ExcludePoleAngle,
                       RealType **spots,
                       int *nspots)

{
    int i, OmegaRangeNo;
    RealType DSpacings[MAX_N_HKLS];
    RealType thetas[MAX_N_HKLS];
    RealType ds;
    RealType theta;
    int KeepSpot;
    for (i = 0 ;i < n_hkls; i++) {
        int sumhkl = hkls[i][0]*hkls[i][0] + hkls[i][1]*hkls[i][1] + hkls[i][2]*hkls[i][2];
        ds = sqrt(LatticeConstant*LatticeConstant / sumhkl);
        theta = rad2deg * asin(Wavelength/(2*ds));
        DSpacings[i] = ds;
        thetas[i] = theta;
    }
    
    int Ghkl[3];
    int indexhkl;
    RealType Gc[3];
    RealType omegas[4];
    RealType etas[4];
    RealType yl;
    RealType zl;
    int nspotsPlane;
    int spotnr = 0;
    int spotid = 0;
    int ringnr = 0;
    for (indexhkl=0; indexhkl < n_hkls ; indexhkl++)  {
        Ghkl[0] = hkls[indexhkl][0];
        Ghkl[1] = hkls[indexhkl][1];
        Ghkl[2] = hkls[indexhkl][2];
        ringnr = hkls[indexhkl][3];
        RealType RingRadius = RingRadii[ringnr];
        MatrixMult(OrientMatrix,Ghkl, Gc);
        ds    = DSpacings[indexhkl];
        theta = thetas[indexhkl];
        CalcOmega(Gc[0], Gc[1], Gc[2], theta, omegas, etas, &nspotsPlane);
        for (i=0 ; i<nspotsPlane ; i++) {
            RealType Omega = omegas[i];
            RealType Eta = etas[i];
            RealType EtaAbs =  fabs(Eta);
            if ((EtaAbs < ExcludePoleAngle ) || ((180-EtaAbs) < ExcludePoleAngle)) continue;
            CalcSpotPosition(RingRadius, etas[i], &(yl), &(zl));
            for (OmegaRangeNo = 0 ; OmegaRangeNo < NOmegaRanges ; OmegaRangeNo++ ) {
                KeepSpot = 0;
                if ( (Omega > OmegaRange[OmegaRangeNo][0]) &&
                    (Omega < OmegaRange[OmegaRangeNo][1]) &&
                    (yl > BoxSizes[OmegaRangeNo][0]) &&
                    (yl < BoxSizes[OmegaRangeNo][1]) &&
                    (zl > BoxSizes[OmegaRangeNo][2]) &&
                    (zl < BoxSizes[OmegaRangeNo][3]) ) {
                    KeepSpot = 1;
                    break;
                }
            }
            if (KeepSpot==1) {
                //    spots[spotnr][0] = OrientID;
                //                spots[spotnr][0] = spotid;
                //      spots[spotnr][2] = indexhkl;
                //        spots[spotnr][3] = distance;
                spots[spotnr][0] = yl;
                spots[spotnr][1] = zl;
                spots[spotnr][2] = omegas[i];
                //                printf("%f %f %f %f\n",yl,zl,omegas[i],RingRadius);
                //            spots[spotnr][7] = etas[i];
                //              spots[spotnr][8] = theta;
                //                spots[spotnr][9] = ringnr;
                spotnr++;
                spotid++;
            }
        }
    }
    *nspots = spotnr;
}

int
CalcDiffractionSpots(double LatticeConstant, 
	double Wavelength, 
	double Distance, 
	int nRings, 
	double ExcludePoleAngle, 
	const int RingNumbers[MAX_N_OMEGA_RANGES],
	double OmegaRanges[MAX_N_OMEGA_RANGES][2], 
	int NoOfOmegaRanges, 
	double BoxSizes[MAX_N_OMEGA_RANGES][4], 
	int *nTspots, 
	double OrientMatr[3][3], 
	double **TheorSpots)
{
    double RingTtheta[MAX_N_HKLS], RingRadii[nRings+1];
    int RingMult[MAX_N_HKLS];
    int RingHKL[MAX_N_HKLS][3];
    int NrOfRings;
    NrOfRings = nRings;
    *nTspots = 0;
    GenerateHKLsFCC(RingNumbers,NrOfRings, hkls, &n_hkls);
    GenerateRingInfo(LatticeConstant, Wavelength, RingTtheta, RingMult, RingHKL);
    CalcRingRadii(Distance,RingTtheta,RingNumbers,nRings,RingRadii);
    int nTsps;
    if (TheorSpots == NULL ) {
        printf("Memory error: could not allocate memory for output matrix. Memory full?\n");
        return 1;
    }
    CalcDiffrSpots_Furnace(OrientMatr, LatticeConstant, Wavelength, Distance, RingRadii, OmegaRanges, BoxSizes, NoOfOmegaRanges, ExcludePoleAngle, TheorSpots, &nTsps);
    *nTspots = nTsps;
}
