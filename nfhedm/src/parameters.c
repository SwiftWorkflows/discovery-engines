/*
 * parameters.c
 *
 *  Created on: Mar 6, 2014
 *      Author: wozniak
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "Debug.h"
#include "checks.h"
#include "parameters.h"

int
parameters_read(const char *filename, struct parameters *params)
{
    PROFILE_CREATE(parameters_read, profile);
    PROFILE_START(profile);
    FILE *fileParam;
    char aline[1000];
//    struct TParam * Param1;
    printf("Parameters: %s\n", filename);
    fileParam = fopen(filename,"r");
    if (fileParam == NULL) file_not_found(filename);
    char *str, dummy[1000];
    int LowNr;
    while (fgets(aline,1000,fileParam)!=NULL){
        str = "nDistances ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %d", dummy, &params->nLayers);
            break;
        }
    }
    fclose(fileParam);
    fileParam = fopen(filename,"r");
    if (fileParam == NULL) file_not_found(filename);
    // double Lsd[nLayers],ybc[nLayers],zbc[nLayers];
    double *Lsd = malloc(params->nLayers*sizeof(double));
    params->Lsd = Lsd;
    double *ybc = malloc(params->nLayers*sizeof(double));
    params->ybc = ybc;
    double *zbc = malloc(params->nLayers*sizeof(double));
    params->zbc = zbc;
    int cntr=0,countr=0,conter=0;

    params->NoOfOmegaRanges = 0;

    while (fgets(aline,1000,fileParam)!=NULL){
                str = "ReducedFileName ";
                
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %s", dummy, params->fn);
            continue;
        }
                 str = "DataDirectory ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %s", dummy, params->direct);
            continue;
        }
                str = "extReduced ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %s", dummy, params->ext);
            continue;
        }
        str = "Lsd ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
          int n = sscanf(aline,"%s %lf", dummy, &Lsd[cntr]);
          assert(n == 2);
          cntr++;
          continue;
        }
        str = "SpaceGroup ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %d", dummy, &params->SpaceGroup);
            continue;
        }
        str = "MaxRingRad ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->MaxRingRad);
            continue;
        }
        str = "StartNr ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %d", dummy, &params->StartNr);
            continue;
        }
        str = "EndNr ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %d", dummy, &params->EndNr);
            continue;
        }
        str = "NrPixels ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %d", dummy, &params->NrPixels);
            continue;
        }
        str = "ExcludePoleAngle ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->ExcludePoleAngle);
            continue;
        }
        str = "LatticeParameter ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            int n = sscanf(aline,"%s %lf %lf %lf %lf %lf %lf ", dummy,
                   &params->LatticeConstant[0], &params->LatticeConstant[1],
                   &params->LatticeConstant[2], &params->LatticeConstant[3],
                   &params->LatticeConstant[4], &params->LatticeConstant[5]);
            assert(n == 7);
            continue;
        }
        str = "tx ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->tx);
            continue;
        }
        str = "ty ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->ty);
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
            sscanf(aline,"%s %lf", dummy, &params->tz);
            continue;
        }
        str = "OrientTol ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->tol);
            continue;
        }
        str = "MinFracAccept ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->minFracOverlap);
            continue;
        }
        str = "OmegaStart ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->OmegaStart);
            continue;
        }
        str = "OmegaStep ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->OmegaStep);
            continue;
        }
        str = "Wavelength ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
          int n = sscanf(aline,"%s %lf", dummy, &params->Wavelength);
          assert(n == 2);
          continue;
        }
        str = "px ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->px);
            continue;
        }
        str = "LsdTol ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->lsdtol);
            continue;
        }
        str = "LsdRelativeTol ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->lsdtolrel);
            continue;
        }
        str = "BCTol ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf %lf", dummy, &params->bctola, &params->bctolb);
            continue;
        }
        str = "TiltsTol ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf", dummy, &params->tiltstol);
            continue;
        }
        str = "OmegaRange ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf %lf", dummy,
                   &params->OmegaRanges[params->NoOfOmegaRanges][0],
                   &params->OmegaRanges[params->NoOfOmegaRanges][1]);
            params->NoOfOmegaRanges++;
            continue;
        }
        str = "BoxSize ";
        LowNr = strncmp(aline,str,strlen(str));
        if (LowNr==0){
            sscanf(aline,"%s %lf %lf %lf %lf", dummy,
                   &params->BoxSizes[countr][0], &params->BoxSizes[countr][1],
                   &params->BoxSizes[countr][2], &params->BoxSizes[countr][3]);
            countr++;
            continue;
        }
    }
    PROFILE_STOP(profile);
    return 1;
}
