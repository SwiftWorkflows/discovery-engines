/*
 * Convert-Bin-L2T.c
 *
 *  Created on: Apr 29, 2014
 *      Author: justin
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "checks.h"
#include "SharedFuncsFit.h"

#define CHECK(condition, msg) \
{ if (!condition) { printf(msg); return false; } }

#define READ(data, size, count, fp)           \
 { int actual = fread(data, size, count, fp); \
   if (actual != count) {                     \
        printf("short read!\n");              \
        return false;        }}

bool
L2P(const char *filename)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) file_not_found(filename);
    // setvbuf(stdout, NULL, _IOFBF, 4*1024*1024);

    struct Theader head;
    int nElements;

    // 1 dummy
    printf("Dummies\n");
    uint32_t dummy;
    READ(&dummy, sizeof(uint32_t), 1, fp);

    // 5 dummies
    ReadHeader(fp, &head);
    PrintHeader(stdout, &head);
    uint32_t *t_ui32=NULL;
    t_ui32 = malloc(sizeof(uint32_t) * 5);
    READ(t_ui32, sizeof(uint32_t), 5, fp);
    PrintUint32s(stdout, t_ui32, 5);

    // Y positions
    printf("Y positions\n");
    ReadHeader(fp, &head);
    PrintHeader(stdout, &head);
    nElements = (head.DataSize - head.NameSize) / sizeof(uint16_t);
    uint16_t *t_ui16 = malloc(nElements*sizeof(uint16_t));
    READ(t_ui16,sizeof(uint16_t),nElements,fp);
    PrintUint16s(stdout, t_ui16, nElements);

    // Z positions
    printf("Z positions\n");
    ReadHeader(fp,&head);
    PrintHeader(stdout, &head);
    READ(t_ui16,sizeof(uint16_t),nElements,fp);
    PrintUint16s(stdout, t_ui16, nElements);

    // Intensities
    printf("Intensities:\n");
    float32_t *t_f32 = malloc(sizeof(float32_t)*nElements);
    ReadHeader(fp,&head);
    PrintHeader(stdout, &head);
    READ(t_f32,sizeof(float32_t),nElements,fp);
    PrintFloat32s(stdout, t_f32, nElements);

    // Peak IDs
    printf("Peak IDs\n");
    ReadHeader(fp,&head);
    PrintHeader(stdout,&head);
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

    bool result = L2P(filename);
    if (!result) {
        printf("%s failed!\n", argv[0]);
        return 1;
    }
    return 0;
 }
