
/*
 * Convert-Bin-L2T.c
 *
 *  Created on: Apr 29, 2014
 *      Author: justin
 *
 * Creates text version of binary file on stdout
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "checks.h"
#include "SharedFuncsFit.h"

bool
L2T(const char *filename)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) file_not_found(filename);
    // setvbuf(stdout, NULL, _IOFBF, 4*1024*1024);

    struct Theader head;
    int nElements;

    bool b;

    // 1 dummy
    printf("Dummies\n");
    uint32_t dummy;
    READ(&dummy, sizeof(uint32_t), 1, fp);

    // 5 dummies
    b = ReadHeader(fp, &head);
    CHECK(b, "ReadHeader failed (dummies)!");
    PrintHeader(stdout, &head);
    uint32_t *t_ui32=NULL;
    t_ui32 = malloc(sizeof(uint32_t) * 5);
    READ(t_ui32, sizeof(uint32_t), 5, fp);
    PrintUint32s(stdout, t_ui32, 5);

    // Y positions
    printf("Y positions\n");
    b = ReadHeader(fp, &head);
    CHECK(b, "ReadHeader failed (Y)!");
    PrintHeader(stdout, &head);
    nElements = (head.DataSize - head.NameSize) / sizeof(uint16_t);
    uint16_t *t_ui16 = malloc(nElements*sizeof(uint16_t));
    READ(t_ui16,sizeof(uint16_t),nElements,fp);
    PrintUint16s(stdout, t_ui16, nElements);

    // Z positions
    printf("Z positions\n");
    b = ReadHeader(fp,&head);
    CHECK(b, "ReadHeader failed (Z)!");
    PrintHeader(stdout, &head);
    int nCheck = (head.DataSize - head.NameSize) / sizeof(uint16_t);
    CHECK(nCheck == nElements, "size mismatch (Z)!");
    READ(t_ui16,sizeof(uint16_t),nElements,fp);
    PrintUint16s(stdout, t_ui16, nElements);

    // Intensities
    printf("Intensities:\n");
    float32_t *t_f32 = malloc(sizeof(float32_t)*nElements);
    b = ReadHeader(fp,&head);
    CHECK(b, "ReadHeader failed (intensities)!");
    PrintHeader(stdout, &head);
    nCheck = (head.DataSize - head.NameSize) / sizeof(float32_t);
    CHECK(nCheck == nElements, "size mismatch (intensities)!");
    READ(t_f32,sizeof(float32_t),nElements,fp);
    PrintFloat32s(stdout, t_f32, nElements);

    // Peak IDs
    printf("Peak IDs\n");
    b = ReadHeader(fp,&head);
    CHECK(b, "ReadHeader failed (peak IDs)!");
    PrintHeader(stdout,&head);
    nCheck = (head.DataSize - head.NameSize) / sizeof(uint16_t);
    CHECK(nCheck == nElements, "size mismatch (peak IDs)!");
    READ(t_ui16,sizeof(uint16_t),nElements,fp);
    PrintUint16s(stdout, t_ui16, nElements);

    // Finish up
    free(t_ui16);
    free(t_ui32);
    free(t_f32);
    fclose(fp);
    return true;
}

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("requires filename!\n");
        exit(1);
    }
    char* filename = argv[1];

    bool result = L2T(filename);
    if (!result) {
        printf("%s failed!\n", argv[0]);
        return 1;
    }
    return 0;
 }
