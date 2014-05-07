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

#include "SharedFuncsFit.h"

#define deg2rad 0.0174532925199433
#define rad2deg 57.2957795130823
#define RealType double
#define MAX_N_HKLS 1000
#define MAX_N_OMEGA_RANGES 20
#define EPS 0.000000001

#define crossProduct(a,b,c) \
(a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; \
(a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; \
(a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];

#define dot(v,q) \
((v)[0] * (q)[0] + \
(v)[1] * (q)[1] + \
(v)[2] * (q)[2])

#define CalcLength(x,y,z) sqrt((x)*(x) + (y)*(y) + (z)*(z))

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
CalcDiffrSpots_Furnace(
                       RealType OrientMatrix[3][3],
                       RealType distance,
                       RealType OmegaRange[][2],
                       RealType BoxSizes[][4],
                       int NOmegaRanges,
                       int hkls[5000][4],
                       int n_hkls,
                       double Thetas[5000],
                       RealType ExcludePoleAngle,
                       RealType **spots,
                       int *nspots)
{
    int i, OmegaRangeNo;
    RealType theta;
    int KeepSpot;
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
    for (indexhkl=0; indexhkl < n_hkls ; indexhkl++)  {
        Ghkl[0] = hkls[indexhkl][0];
        Ghkl[1] = hkls[indexhkl][1];
        Ghkl[2] = hkls[indexhkl][2];
        RealType RingRadius = distance * tan(2*deg2rad*Thetas[indexhkl]);
        MatrixMult(OrientMatrix,Ghkl, Gc);
        theta = Thetas[indexhkl];
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
                spots[spotnr][0] = yl;
                spots[spotnr][1] = zl;
                spots[spotnr][2] = omegas[i];
                spotnr++;
                spotid++;
            }
        }
    }
    *nspots = spotnr;
}

int
CalcDiffractionSpots(double Distance, 
	double ExcludePoleAngle, 
	double OmegaRanges[MAX_N_OMEGA_RANGES][2], 
	int NoOfOmegaRanges,
	int hkls[5000][4],
	int n_hkls,
	double Thetas[5000],
	double BoxSizes[MAX_N_OMEGA_RANGES][4], 
	int *nTspots, 
	double OrientMatr[3][3], 
	double **TheorSpots)
{
    *nTspots = 0;
    int nTsps;
    if (TheorSpots == NULL ) {
        printf("Memory error: could not allocate memory for output matrix. Memory full?\n");
        return 1;
    }
    CalcDiffrSpots_Furnace(OrientMatr, Distance, OmegaRanges, BoxSizes,
		NoOfOmegaRanges, hkls, n_hkls, Thetas, ExcludePoleAngle, TheorSpots, &nTsps);
    *nTspots = nTsps;
    return 0;
}
