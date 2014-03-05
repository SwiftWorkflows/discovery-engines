/*
 * math.c
 *
 *  Created on: Mar 5, 2014
 *      Author: wozniak
 */

#include "nfhedm-math.h"

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
