
#include <assert.h>
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

#include "SharedFuncsFit.h"
#include "FitOrientation.h"

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

static void
usage()
{
    printf("usage: fo-nlopt <PARAMETERS> <GRID-POINT-NUMBER>\n");
}

int
main(int argc, char *argv[])
{
    if (argc < 3)
    {
        usage();
        return EXIT_FAILURE;
    }

    // Read params file.
    char *ParamFN;
    ParamFN = argv[1];
    //Read position.
    int rown=atoi(argv[2]);
	double result[4];

    FitOrientationAll(ParamFN, rown, result);

    return 0;
}
