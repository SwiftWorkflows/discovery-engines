/*
 * swift-bindings.c
 *
 *  Created on: Mar 18, 2014
 *      Author: wozniak
 */

#include "FitOrientation.h"

#include "swift-bindings.h"

int FitOrientation_Swift(const char *ParameterFileName, int rown, char *MicrostructureData)
{
    return FitOrientationAll(ParameterFileName, rown, MicrostructureData);
}
