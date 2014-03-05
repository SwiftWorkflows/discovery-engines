/*
 * nfhedm-math.h
 *
 *  Created on: Mar 5, 2014
 *      Author: wozniak
 */

#ifndef NFHEDMMATH_H
#define NFHEDMMATH_H

#include "math.h"

#define RealType double

#define deg2rad 0.0174532925199433
#define rad2deg 57.2957795130823

void
MatrixMultF(
    RealType m[3][3],
    RealType v[3],
    RealType r[3]);

void
RotateAroundZ(
    RealType v1[3],
    RealType alpha,
    RealType v2[3]);

#endif
