/*
 * Convert-Bin-L2N.c
 *
 *  Created on: Apr 29, 2014
 *      Author: justin
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "checks.h"
#include "SharedFuncsFit.h"

bool
L2N(const char *input, const char *output)
{
    FILE* fp_in = fopen(input, "r");
    if (fp_in == NULL) file_not_found(input);
    FILE* fp_out = fopen(output, "w");
    if (fp_in == NULL) file_not_writable(output);
    // setvbuf(stdout, NULL, _IOFBF, 4*1024*1024);

    struct Theader head;
    int nElements;

    bool b;

    // 1 dummy
    printf("Dummies\n");
    uint32_t dummy;
    READ(&dummy, sizeof(uint32_t), 1, fp_in);
    hton_Uint32s(&dummy, 1);
    fwrite(&dummy, sizeof(uint32_t), 1, fp_out);

    // 5 dummies
    b = ReadHeader(fp_in, &head);
    CHECK(b, "ReadHeader failed (dummies)!");
    NWriteHeader(fp_out, &head);
    uint32_t *t_ui32=NULL;
    t_ui32 = malloc(sizeof(uint32_t) * 5);
    READ(t_ui32, sizeof(uint32_t), 5, fp_in);
    hton_Uint32s(t_ui32, 5);
    fwrite(t_ui32, sizeof(uint32_t), 5, fp_out);

    // Y positions
    printf("Y positions\n");
    b = ReadHeader(fp_in, &head);
    CHECK(b, "ReadHeader failed (Y)!");
    NWriteHeader(fp_out, &head);
    nElements = (head.DataSize - head.NameSize) / sizeof(uint16_t);
    uint16_t *t_ui16 = malloc(nElements*sizeof(uint16_t));
    READ(t_ui16,sizeof(uint16_t),nElements,fp_in);
    hton_Uint16s(t_ui16, nElements);
    fwrite(t_ui16, sizeof(uint16_t), nElements, fp_out);

    // Z positions
    printf("Z positions\n");
    b = ReadHeader(fp_in,&head);
    CHECK(b, "ReadHeader failed (Z)!");
    NWriteHeader(fp_out, &head);
    int nCheck = (head.DataSize - head.NameSize) / sizeof(uint16_t);
    CHECK(nCheck == nElements, "size mismatch (Z)!");
    READ(t_ui16,sizeof(uint16_t),nElements,fp_in);
    hton_Uint16s(t_ui16, nElements);
    fwrite(t_ui16, sizeof(uint16_t), nElements, fp_out);

    // Intensities
    printf("Intensities\n");
    float32_t *t_f32 = malloc(sizeof(float32_t)*nElements);
    b = ReadHeader(fp_in,&head);
    CHECK(b, "ReadHeader failed (intensities)!");
    NWriteHeader(fp_out, &head);
    nCheck = (head.DataSize - head.NameSize) / sizeof(float32_t);
    CHECK(nCheck == nElements, "size mismatch (intensities)!");
    READ(t_f32,sizeof(float32_t),nElements,fp_in);
    hton_Float32s(t_f32, nElements);
    fwrite(t_f32, sizeof(float32_t), nElements, fp_out);

    // Peak IDs
    printf("Peak IDs\n");
    b = ReadHeader(fp_in,&head);
    CHECK(b, "ReadHeader failed (peak IDs)!");
    NWriteHeader(fp_out,&head);
    nCheck = (head.DataSize - head.NameSize) / sizeof(uint16_t);
    CHECK(nCheck == nElements, "size mismatch (peak IDs)!");
    READ(t_ui16,sizeof(uint16_t),nElements,fp_in);
    hton_Uint16s(t_ui16, nElements);
    fwrite(t_ui16, sizeof(uint16_t), nElements, fp_out);

    // Finish up
    free(t_ui16);
    free(t_ui32);
    free(t_f32);
    fclose(fp_in);
    fclose(fp_out);
    return true;
}

int
main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("requires 2 filenames!\n");
        exit(1);
    }
    char* input  = argv[1];
    char* output = argv[2];

    bool result = L2N(input, output);
    if (!result) {
        printf("%s failed!\n", argv[0]);
        return 1;
    }
    return 0;
 }
