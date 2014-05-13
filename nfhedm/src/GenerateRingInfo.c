// Code to calculate ring information for a given space-group number
//
//  Hemant Sharma
//  2014/04/08
//
//  Based on the Fable tools written originally by H.O. Sorensen

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void CalcSGConditions(int sgnum, 
	int sgconditions[23])
{
	int i=0;
	for (i=0;i<22;i++) sgconditions[i]=0;
	switch (sgnum) {
		case 4:
			sgconditions[21] = 2;
			break;
		case 5:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 7:
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 8:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 9:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 11:
			sgconditions[21] = 2;
			break;
		case 12:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 13:
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 14:
			sgconditions[14] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 15:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 17:
			sgconditions[22] = 2;
			break;
		case 18:
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 19:
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 20:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 21:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 22:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 23:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 24:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 26:
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 27:
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 28:
			sgconditions[13] = 2;
			sgconditions[20] = 2;
			break;
		case 29:
			sgconditions[11] = 2;
			sgconditions[13] = 2;
			sgconditions[20] = 2;
			sgconditions[22] = 2;
			break;
		case 30:
			sgconditions[12] = 2;
			sgconditions[14] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 31:
			sgconditions[15] = 2;
			sgconditions[20] = 2;
			sgconditions[22] = 2;
			break;
		case 32:
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 33:
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 34:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 35:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 36:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 37:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 38:
			sgconditions[2] = 2;
			sgconditions[12] = 2;
			sgconditions[14] = 2;
			sgconditions[17] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 39:
			sgconditions[2] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[14] = 2;
			sgconditions[17] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 40:
			sgconditions[2] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[17] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 41:
			sgconditions[2] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[17] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 42:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 43:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 4;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 4;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 4;
			sgconditions[21] = 4;
			sgconditions[22] = 4;
			break;
		case 44:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 45:
			sgconditions[4] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 46:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 48:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 49:
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 50:
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 51:
			sgconditions[16] = 2;
			sgconditions[20] = 2;
			break;
		case 52:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 53:
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[20] = 2;
			sgconditions[22] = 2;
			break;
		case 54:
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[16] = 2;
			sgconditions[20] = 2;
			sgconditions[22] = 2;
			break;
		case 55:
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 56:
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 57:
			sgconditions[10] = 2;
			sgconditions[14] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 58:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 59:
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 60:
			sgconditions[10] = 2;
			sgconditions[14] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 61:
			sgconditions[10] = 2;
			sgconditions[14] = 2;
			sgconditions[16] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 62:
			sgconditions[12] = 2;
			sgconditions[16] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 63:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 64:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 65:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 66:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 67:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 68:
			sgconditions[0] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 69:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 70:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 4;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 4;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 4;
			sgconditions[20] = 4;
			sgconditions[21] = 4;
			sgconditions[22] = 4;
			break;
		case 71:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 72:
			sgconditions[4] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 73:
			sgconditions[4] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 74:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 76:
			sgconditions[22] = 4;
			break;
		case 77:
			sgconditions[22] = 2;
			break;
		case 78:
			sgconditions[22] = 4;
			break;
		case 79:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 80:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 4;
			break;
		case 82:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 84:
			sgconditions[22] = 2;
			break;
		case 85:
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 86:
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 87:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 88:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 4;
			break;
		case 90:
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 91:
			sgconditions[22] = 4;
			break;
		case 92:
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 4;
			break;
		case 93:
			sgconditions[22] = 2;
			break;
		case 94:
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 95:
			sgconditions[22] = 4;
			break;
		case 96:
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 4;
			break;
		case 97:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 98:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 4;
			break;
		case 100:
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 101:
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 102:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 103:
			sgconditions[7] = 2;
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 104:
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 105:
			sgconditions[7] = 2;
			sgconditions[22] = 2;
			break;
		case 106:
			sgconditions[7] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 107:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 108:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 109:
			sgconditions[4] = 2;
			sgconditions[9] = 4;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 4;
			break;
		case 110:
			sgconditions[4] = 2;
			sgconditions[9] = 4;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 4;
			break;
		case 112:
			sgconditions[7] = 2;
			sgconditions[22] = 2;
			break;
		case 113:
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 114:
			sgconditions[7] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 116:
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 117:
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 118:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 119:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 120:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 121:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 122:
			sgconditions[4] = 2;
			sgconditions[9] = 4;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 4;
			break;
		case 124:
			sgconditions[7] = 2;
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 125:
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 126:
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 127:
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 128:
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 129:
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			break;
		case 130:
			sgconditions[7] = 2;
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 131:
			sgconditions[7] = 2;
			sgconditions[22] = 2;
			break;
		case 132:
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 133:
			sgconditions[7] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 134:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
 			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 135:
			sgconditions[7] = 2;
			sgconditions[10] = 2;
			sgconditions[13] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 136:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 137:
			sgconditions[7] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 138:
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 139:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 140:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 141:
			sgconditions[4] = 2;
			sgconditions[9] = 4;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 4;
			break;
		case 142:
			sgconditions[4] = 2;
			sgconditions[9] = 4;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 4;
			break;
		case 144:
			sgconditions[22] = 3;
			break;
		case 145:
			sgconditions[22] = 3;
			break;
		case 146:
			sgconditions[5] = 3;
			break;
		case 148:
			sgconditions[5] = 3;
			break;
		case 151:
			sgconditions[22] = 3;
			break;
		case 152:
			sgconditions[22] = 3;
			break;
		case 153:
			sgconditions[22] = 3;
			break;
		case 154:
			sgconditions[22] = 3;
			break;
		case 155:
			sgconditions[5] = 3;
			break;
		case 158:
			sgconditions[22] = 2;
			break;
		case 159:
			sgconditions[7] = 2;
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 160:
			sgconditions[5] = 2;
			break;
		case 161:
			sgconditions[5] = 2;
			break;
		case 163:
			sgconditions[7] = 2;
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 165:
			sgconditions[22] = 2;
			break;
		case 166:
			sgconditions[5] = 3;
			break;
		case 167:
			sgconditions[5] = 3;
			break;
		case 169:
			sgconditions[22] = 6;
			break;
		case 170:
			sgconditions[22] = 6;
			break;
		case 171:
			sgconditions[22] = 3;
			break;
		case 172:
			sgconditions[22] = 3;
			break;
		case 173:
			sgconditions[22] = 2;
			break;
		case 176:
			sgconditions[22] = 2;
			break;
		case 178:
			sgconditions[22] = 6;
			break;
		case 179:
			sgconditions[22] = 6;
			break;
		case 180:
			sgconditions[22] = 3;
			break;
		case 181:
			sgconditions[22] = 3;
			break;
		case 182:
			sgconditions[22] = 2;
			break;
		case 184:
			sgconditions[7] = 2;
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 185:
			sgconditions[22] = 2;
			break;
		case 186:
			sgconditions[7] = 2;
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 188:
			sgconditions[22] = 2;
			break;
		case 190:
			sgconditions[7] = 2;
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 192:
			sgconditions[7] = 2;
			sgconditions[11] = 2;
			sgconditions[14] = 2;
			sgconditions[22] = 2;
			break;
		case 193:
			sgconditions[11] = 2;
			sgconditions[22] = 2;
			break;
		case 194:
			sgconditions[7] = 2;
			sgconditions[22] = 2;
			break;
		case 196:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[8] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 197:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 198:
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 199:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 201:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 202:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[8] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 203:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[8] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 4;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 4;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 4;
			sgconditions[20] = 4;
			sgconditions[21] = 4;
			sgconditions[22] = 4;
			break;
		case 204:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 205:
			sgconditions[10] = 2;
			sgconditions[14] = 2;
			sgconditions[16] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 206:
			sgconditions[4] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 208:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 209:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[8] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 210:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[8] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 4;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 4;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 4;
			sgconditions[21] = 4;
			sgconditions[22] = 4;
			break;
		case 211:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 212:
			sgconditions[12] = 4;
			sgconditions[15] = 4;
			sgconditions[19] = 2;
			sgconditions[20] = 4;
			sgconditions[21] = 4;
			sgconditions[22] = 4;
			break;
		case 213:
			sgconditions[12] = 4;
			sgconditions[15] = 4;
			sgconditions[19] = 4;
			sgconditions[20] = 4;
			sgconditions[21] = 4;
			sgconditions[22] = 4;
			break;
		case 214:
			sgconditions[4] = 2;
			sgconditions[12] = 4;
			sgconditions[15] = 4;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 4;
			sgconditions[21] = 4;
			sgconditions[22] = 4;
			break;
		case 216:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[8] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 217:
			sgconditions[4] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 218:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 219:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[6] = 2;
			sgconditions[7] = 2;
			sgconditions[8] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 220:
			sgconditions[4] = 2;
			sgconditions[9] = 4;
			sgconditions[12] = 4;
			sgconditions[15] = 4;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 4;
			sgconditions[21] = 4;
			sgconditions[22] = 4;
			break;
		case 222:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 223:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 224:
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 225:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[8] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 226:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[6] = 2;
			sgconditions[7] = 2;
			sgconditions[8] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 2;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 2;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 227:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[8] = 4;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 4;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 4;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 4;
			sgconditions[19] = 4;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 228:
			sgconditions[0] = 2;
			sgconditions[1] = 2;
			sgconditions[2] = 2;
			sgconditions[3] = 2;
			sgconditions[6] = 2;
			sgconditions[7] = 2;
			sgconditions[8] = 2;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 4;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 4;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 4;
			sgconditions[19] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 229:
			sgconditions[4] = 2;
			sgconditions[7] = 2;
			sgconditions[12] = 2;
			sgconditions[15] = 2;
			sgconditions[18] = 2;
			sgconditions[20] = 2;
			sgconditions[21] = 2;
			sgconditions[22] = 2;
			break;
		case 230:
			sgconditions[4] = 2;
			sgconditions[9] = 4;
			sgconditions[10] = 2;
			sgconditions[11] = 2;
			sgconditions[12] = 4;
			sgconditions[13] = 2;
			sgconditions[14] = 2;
			sgconditions[15] = 4;
			sgconditions[16] = 2;
			sgconditions[17] = 2;
			sgconditions[18] = 2;
			sgconditions[19] = 4;
			sgconditions[20] = 4;
			sgconditions[21] = 4;
			sgconditions[22] = 4;
			break;
		default:
			break;
	}
}

double Theta(double a, double b, double c, double alpha,
	double beta, double gamma, double wavelength, int hkl[3])
{
	double cosAlpha = cos(alpha*(M_PI/180.0));
	double cosBeta  = cos(beta *(M_PI/180.0));
	double cosGamma = cos(gamma*(M_PI/180.0));
	double h = (double) hkl[0];
	double k = (double) hkl[1];
	double l = (double) hkl[2];
	double numer =  (((h*h)/(a*a))*(1-(cosAlpha*cosAlpha)))+
	                (((k*k)/(b*b))*(1-(cosBeta *cosBeta )))+
	                (((l*l)/(c*c))*(1-(cosGamma*cosGamma)))+
	                (2*h*k*(((cosAlpha*cosBeta )-cosGamma)/(a*b)))+
	                (2*h*l*(((cosAlpha*cosGamma)-cosBeta )/(a*c)))+
	                (2*k*l*(((cosGamma*cosBeta )-cosAlpha)/(b*c)));
	double denom =  1.0 - ((cosAlpha*cosAlpha) + (cosBeta*cosBeta) +
					(cosGamma*cosGamma)) + (2*cosAlpha*cosBeta*cosGamma);
	double sinThetaPerLambda = ((sqrt(numer))/(2.0*sqrt(denom)));
	return asin(wavelength*sinThetaPerLambda)*180/M_PI;
}

int SystematicAbsentUnique(int hkl[3], int SGConditions[23])
{
	int h = hkl[0];
	int k = hkl[1];
	int l = hkl[2];
	int SystematicAbsentType = 0;
	int condition;
	
	if (SGConditions[0]!=0){
		condition = SGConditions[0];
		if (((abs(h+k))%condition)!=0) SystematicAbsentType = 1;
	}
	if (SGConditions[1]!=0){
		condition = SGConditions[1];
		if (((abs(h+l))%condition)!=0) SystematicAbsentType = 2;
	}	
	if (SGConditions[2]!=0){
		condition = SGConditions[2];
		if (((abs(k+l))%condition)!=0) SystematicAbsentType = 3;
	}
	if (SGConditions[3]!=0){
		SystematicAbsentType = 4;
		condition = SGConditions[3];
		if (((abs(h+k))%condition)==0){
			if (((abs(h+l))%condition)==0){
				if (((abs(k+l))%condition)==0){
					SystematicAbsentType = 0;
				}
			}
		}
	}
	if (SGConditions[4]!=0){
		condition = SGConditions[4];
		if (((abs(h+k+l))%condition)!=0) SystematicAbsentType = 5;
	}
	if (SGConditions[5]!=0){
		condition = SGConditions[5];
		if (((abs(-h+k+l))%condition)!=0) SystematicAbsentType = 6;
	}
	if ((h-k)==0){
		if (SGConditions[6]!=0){
			condition = SGConditions[6];
			if (((abs(h))%condition)!=0) SystematicAbsentType = 7;
		}
		if (SGConditions[7]!=0){
			condition = SGConditions[7];
			if (((abs(l))%condition)!=0) SystematicAbsentType = 8;
		}
		if (SGConditions[8]!=0){
			condition = SGConditions[8];
			if (((abs(h+l))%condition)!=0) SystematicAbsentType = 9;
		}
		if (SGConditions[9]!=0){
			condition = SGConditions[9];
			if (((abs(h+h+l))%condition)!=0) SystematicAbsentType = 10;
		}
	}
	if (h==0){
		if (SGConditions[10]!=0){
			condition = SGConditions[10];
			if (((abs(k))%condition)!=0) SystematicAbsentType = 11;
		}
		if (SGConditions[11]!=0){
			condition = SGConditions[11];
			if (((abs(l))%condition)!=0) SystematicAbsentType = 12;
		}
		if (SGConditions[12]!=0){
			condition = SGConditions[12];
			if (((abs(k+l))%condition)!=0) SystematicAbsentType = 13;
		}
	}
	if (k==0){
		if (SGConditions[13]!=0){
			condition = SGConditions[13];
			if (((abs(h))%condition)!=0) SystematicAbsentType = 14;
		}
		if (SGConditions[14]!=0){
			condition = SGConditions[14];
			if (((abs(l))%condition)!=0) SystematicAbsentType = 15;
		}
		if (SGConditions[15]!=0){
			condition = SGConditions[15];
			if (((abs(h+l))%condition)!=0) SystematicAbsentType = 16;
		}
	}
	if (l==0){
		if (SGConditions[16]!=0){
			condition = SGConditions[16];
			if (((abs(h))%condition)!=0) SystematicAbsentType = 17;
		}
		if (SGConditions[17]!=0){
			condition = SGConditions[17];
			if (((abs(k))%condition)!=0) SystematicAbsentType = 18;
		}
		if (SGConditions[18]!=0){
			condition = SGConditions[18];
			if (((abs(h+k))%condition)!=0) SystematicAbsentType = 19;
		}
	}
	if (l==0 && ((h-k)==0) && SGConditions[19]!=0 
		&& (abs(h) % SGConditions[19]!=0)) SystematicAbsentType = 20;
	if ((abs(k)+ abs(l)) == 0 && SGConditions[20]!=0 
		&& (abs(h) % SGConditions[20]!=0)) SystematicAbsentType = 21;
	if ((abs(h)+ abs(l)) == 0 && SGConditions[21]!=0 
		&& (abs(k) % SGConditions[21]!=0)) SystematicAbsentType = 22;
	if ((abs(h)+ abs(k)) == 0 && SGConditions[22]!=0 
		&& (abs(l) % SGConditions[22]!=0)) SystematicAbsentType = 23;

	return SystematicAbsentType;
}

int SystematicAbsent(int hkl[3], int SGConditions[23], int CrystSystem)
{
	int SysType;
	SysType = SystematicAbsentUnique(hkl, SGConditions);
	if (CrystSystem == 1){
		if (SysType ==0){
			int hkl2[3];
			hkl2[0] = -(hkl[0]+hkl[1]);
			hkl2[1] = hkl[0];
			hkl2[2] = hkl[2];
			SysType = SystematicAbsentUnique(hkl2, SGConditions);
			if (SysType == 0){
				hkl2[0] = hkl[1];
				hkl2[1] = -(hkl[0]+hkl[1]);
				hkl2[2] = hkl[2];
				SysType = SystematicAbsentUnique(hkl2,SGConditions);
			}
		}
	}
	return SysType;
}

void GenPlaneNOO(int n, int Plane[18]){
	int i;
	for (i=0;i<18;i++) Plane[i] = 0;
	Plane[0] = -n;
	Plane[4] = -n;
	Plane[8] = -n;
	Plane[11] = n;
	Plane[13] = n;
	Plane[15] = n;
}

void GenPlaneNNO(int n, int Plane[36]){
	int i;
	for (i=0;i<36;i++) Plane[i] = 0;
	Plane[0] = -n;
	Plane[1] = -n;
	Plane[3] = -n;
	Plane[5] = -n;
	Plane[6] = -n;
	Plane[8] =  n;
	Plane[9] = -n;
	Plane[10] = n;
	Plane[13] = -n;
	Plane[14] = -n;
	Plane[16] = -n;
	Plane[17] = n;
	Plane[19] = n;
	Plane[20] = -n;
	Plane[22] = n;
	Plane[23] = n;
	Plane[24] = n;
	Plane[25] = -n;
	Plane[27] = n;
	Plane[29] = -n;
	Plane[30] = n;
	Plane[32] = n;
	Plane[33] = n;
	Plane[34] = n;
}

void GenPlaneNNN(int n, int Plane[24]){
	Plane[0] = -n;
	Plane[1] = -n;
	Plane[2] = -n;
	Plane[3] = n;
	Plane[4] = n;
	Plane[5] = n;
	Plane[6] = -n;
	Plane[7] = -n;
	Plane[8] = n;
	Plane[9] = -n;
	Plane[10] = n;
	Plane[11] = -n;
	Plane[12] = -n;
	Plane[13] = n;
	Plane[14] = n;
	Plane[15] = n;
	Plane[16] = -n;
	Plane[17] = -n;
	Plane[18] = n;
	Plane[19] = -n;
	Plane[20] = n;
	Plane[21] = n;
	Plane[22] = n;
	Plane[23] = -n;
}

void GenPlaneMNN(int m, int n, int Plane[72]){
	Plane[0] = -m;
	Plane[1] = -n;
	Plane[2] = -n;
	Plane[3] = -n;
	Plane[4] = -m;
	Plane[5] = -n;
	Plane[6] = n;
	Plane[7] = m;
	Plane[8] = n;
	Plane[9] = m;
	Plane[10] = n;
	Plane[11] = n;
	Plane[12] = -m;
	Plane[13] = -n;
	Plane[14] = n;
	Plane[15] = -m;
	Plane[16] = n;
	Plane[17] = -n;
	Plane[18] = -m;
	Plane[19] = n;
	Plane[20] = n;
	Plane[21] = -n;
	Plane[22] = -m;
	Plane[23] = n;
	Plane[24] = -n;
	Plane[25] = -n;
	Plane[26] = -m;
	Plane[27] = -n;
	Plane[28] = -n;
	Plane[29] = m;
	Plane[30] = -n;
	Plane[31] = n;
	Plane[32] = -m;
	Plane[33] = -n;
	Plane[34] = n;
	Plane[35] = m;
	Plane[36] = -n;
	Plane[37] = m;
	Plane[38] = -n;
	Plane[39] = -n;
	Plane[40] = m;
	Plane[41] = n;
	Plane[42] = n;
	Plane[43] = -m;
	Plane[44] = -n;
	Plane[45] = n;
	Plane[46] = -m;
	Plane[47] = n;
	Plane[48] = n;
	Plane[49] = -n;
	Plane[50] = -m;
	Plane[51] = n;
	Plane[52] = -n;
	Plane[53] = m;
	Plane[54]= n;
	Plane[55] = n;
	Plane[56] = -m;
	Plane[57] = n;
	Plane[58] = n;
	Plane[59] = m;
	Plane[60] = n;
	Plane[61] = m;
	Plane[62] = -n;
	Plane[63] = m;
	Plane[64] = -n;
	Plane[65] = -n;
	Plane[66] = m;
	Plane[67] = -n;
	Plane[68] = n;
	Plane[69] = m;
	Plane[70] = n;
	Plane[71] = -n;
}

void GenPlaneMNO(int m, int n, int Plane[72]){
	Plane[0] = -m;
	Plane[1] = -n;
	Plane[2] = 0;
	Plane[3] = -m;
	Plane[4] = 0;
	Plane[5] = -n;
	Plane[6] = -m;
	Plane[7] = n;
	Plane[8] = 0;
	Plane[9] = -m;
	Plane[10] = 0;
	Plane[11] = n;
	Plane[12] = -n;
	Plane[13] = -m;
	Plane[14] = 0;
	Plane[15] = -n;
	Plane[16] = 0;
	Plane[17] = -m;
	Plane[18] = -n;
	Plane[19] = m;
	Plane[20] = 0;
	Plane[21] = -n;
	Plane[22] = 0;
	Plane[23] = m;
	Plane[24] = m;
	Plane[25] = n;
	Plane[26] = 0;
	Plane[27] = m;
	Plane[28] = 0;
	Plane[29] = n;
	Plane[30] = m;
	Plane[31] = -n;
	Plane[32] = 0;
	Plane[33] = m;
	Plane[34] = 0;
	Plane[35] = -n;
	Plane[36] = n;
	Plane[37] = m;
	Plane[38] = 0;
	Plane[39] = n;
	Plane[40] = 0;
	Plane[41] = m;
	Plane[42] = n;
	Plane[43] = -m;
	Plane[44] = 0;
	Plane[45] = n;
	Plane[46] = 0;
	Plane[47] = -m;
	Plane[48] = 0;
	Plane[49] = -n;
	Plane[50] = -m;
	Plane[51] = 0;
	Plane[52] = -m;
	Plane[53] = -n;
	Plane[54]= 0;
	Plane[55] = m;
	Plane[56] = n;
	Plane[57] = 0;
	Plane[58] = n;
	Plane[59] = m;
	Plane[60] = 0;
	Plane[61] = -m;
	Plane[62] = n;
	Plane[63] = 0;
	Plane[64] = n;
	Plane[65] = -m;
	Plane[66] = 0;
	Plane[67] = m;
	Plane[68] = -n;
	Plane[69] = 0;
	Plane[70] = -n;
	Plane[71] = m;
}

void GenPlaneMNP(int m, int n, int p, int Plane[144]){
	Plane[0] = -m;
	Plane[1] = -n;
	Plane[2] = p;
	Plane[3] = -m;
	Plane[4] = p;
	Plane[5] = -n;
	Plane[6] = -m;
	Plane[7] = n;
	Plane[8] = p;
	Plane[9] = -m;
	Plane[10] = p;
	Plane[11] = n;
	Plane[12] = -n;
	Plane[13] = -m;
	Plane[14] = p;
	Plane[15] = -n;
	Plane[16] = p;
	Plane[17] = -m;
	Plane[18] = -n;
	Plane[19] = m;
	Plane[20] = p;
	Plane[21] = -n;
	Plane[22] = p;
	Plane[23] = m;
	Plane[24] = m;
	Plane[25] = n;
	Plane[26] = p;
	Plane[27] = m;
	Plane[28] = p;
	Plane[29] = n;
	Plane[30] = m;
	Plane[31] = -n;
	Plane[32] = p;
	Plane[33] = m;
	Plane[34] = p;
	Plane[35] = -n;
	Plane[36] = n;
	Plane[37] = m;
	Plane[38] = p;
	Plane[39] = n;
	Plane[40] = p;
	Plane[41] = m;
	Plane[42] = n;
	Plane[43] = -m;
	Plane[44] = p;
	Plane[45] = n;
	Plane[46] = p;
	Plane[47] = -m;
	Plane[48] = p;
	Plane[49] = -n;
	Plane[50] = -m;
	Plane[51] = p;
	Plane[52] = -m;
	Plane[53] = -n;
	Plane[54]= p;
	Plane[55] = m;
	Plane[56] = n;
	Plane[57] = p;
	Plane[58] = n;
	Plane[59] = m;
	Plane[60] = p;
	Plane[61] = -m;
	Plane[62] = n;
	Plane[63] = p;
	Plane[64] = n;
	Plane[65] = -m;
	Plane[66] = p;
	Plane[67] = m;
	Plane[68] = -n;
	Plane[69] = p;
	Plane[70] = -n;
	Plane[71] = m;
	Plane[72] = -m;
	Plane[73] = -n;
	Plane[74] = -p;
	Plane[75] = -m;
	Plane[76] = -p;
	Plane[77] = -n;
	Plane[78] = -m;
	Plane[79] = n;
	Plane[80] = -p;
	Plane[81] = -m;
	Plane[82] = -p;
	Plane[83] = n;
	Plane[84] = -n;
	Plane[85] = -m;
	Plane[86] = -p;
	Plane[87] = -n;
	Plane[88] = -p;
	Plane[89] = -m;
	Plane[90] = -n;
	Plane[91] = m;
	Plane[92] = -p;
	Plane[93] = -n;
	Plane[94] = -p;
	Plane[95] = m;
	Plane[96] = m;
	Plane[97] = n;
	Plane[98] = -p;
	Plane[99] = m;
	Plane[100] = -p;
	Plane[101] = n;
	Plane[102] = m;
	Plane[103] = -n;
	Plane[104] = -p;
	Plane[105] = m;
	Plane[106] = -p;
	Plane[107] = -n;
	Plane[108] = n;
	Plane[109] = m;
	Plane[110] = -p;
	Plane[111] = n;
	Plane[112] = -p;
	Plane[113] = m;
	Plane[114] = n;
	Plane[115] = -m;
	Plane[116] = -p;
	Plane[117] = n;
	Plane[118] = -p;
	Plane[119] = -m;
	Plane[120] = -p;
	Plane[121] = -n;
	Plane[122] = -m;
	Plane[123] = -p;
	Plane[124] = -m;
	Plane[125] = -n;
	Plane[126] = -p;
	Plane[127] = m;
	Plane[128] = n;
	Plane[129] = -p;
	Plane[130] = n;
	Plane[131] = m;
	Plane[132] = -p;
	Plane[133] = -m;
	Plane[134] = n;
	Plane[135] = -p;
	Plane[136] = n;
	Plane[137] = -m;
	Plane[138] = -p;
	Plane[139] = m;
	Plane[140] = -n;
	Plane[141] = -p;
	Plane[142] = -n;
	Plane[143] = m;
}

int GenPlanes(int hkl[3],int PlanesOut[48][3]){
	int nPlanes, i,j;
	for (i=0;i<48;i++){
		for (j=0;j<3;j++) PlanesOut[i][j] = 0;
	}
	if (hkl[1] == 0 && hkl[2] == 0){
		int Planes[18];
		GenPlaneNOO(hkl[0],Planes);
		nPlanes = 6;
		for (i = 0;i<nPlanes;i++){
			for (j=0;j<3;j++) PlanesOut[i][j] = Planes[(i*3)+j];
		}
	}else if (hkl[2] == 0 && hkl[1] == hkl[0]){
		int Planes[36];
		GenPlaneNNO(hkl[0],Planes);
		nPlanes = 12;
		for (i = 0;i<nPlanes;i++){
			for (j=0;j<3;j++) PlanesOut[i][j] = Planes[(i*3)+j];
		}
	}else if (hkl[0] == hkl[1] && hkl[1] == hkl[2]){
		int Planes[24];
		GenPlaneNNN(hkl[0],Planes);
		nPlanes = 8;
		for (i = 0;i<nPlanes;i++){
			for (j=0;j<3;j++) PlanesOut[i][j] = Planes[(i*3)+j];
		}
	}else if (hkl[1] == hkl[2]){
		int Planes[72];
		GenPlaneMNN(hkl[0],hkl[1],Planes);
		nPlanes = 24;
		for (i = 0;i<nPlanes;i++){
			for (j=0;j<3;j++) PlanesOut[i][j] = Planes[(i*3)+j];
		}
	}else if (hkl[2] == 0){
		int Planes[72];
		GenPlaneMNO(hkl[0],hkl[1],Planes);
		nPlanes = 24;
		for (i = 0;i<nPlanes;i++){
			for (j=0;j<3;j++) PlanesOut[i][j] = Planes[(i*3)+j];
		}
	}else {
		int Planes[144];
		GenPlaneMNP(hkl[0],hkl[1],hkl[2],Planes);
		nPlanes = 48;
		for (i = 0;i<nPlanes;i++){
			for (j=0;j<3;j++) PlanesOut[i][j] = Planes[(i*3)+j];
		}
	}
	return nPlanes;
}

void GenerateHKLs(int SpaceGroup, double a, double b, double c, 
	double alpha, double beta, double gamma, double wavelength,
	double ThetaMax, int PlanesOut[14000][3], double ThetaOut[14000],
	int *TotPlanes)
{
	int CrystSystem = 0;
	if (SpaceGroup > 142 && SpaceGroup < 195) CrystSystem = 1;
	int SGConditions[23];
	CalcSGConditions(SpaceGroup,SGConditions);
	int i,j,h,k,l,hkl[3],nPlanes,Planes[48][3],ThisPlane[3],nPlanesFilled=0;
	double Th;
	for (h=1;h<15;h++){
		for (k=0;k<=h;k++){
			for (l=0;l<=k;l++){
				hkl[0] = h; hkl[1] = k; hkl[2] = l;
				nPlanes = GenPlanes(hkl,Planes);
				for (i=0;i<nPlanes;i++){
					for (j=0;j<3;j++){
						ThisPlane[j] = Planes[i][j];
					}
					if (SystematicAbsent(ThisPlane,SGConditions,CrystSystem) == 0){
						Th = Theta(a,b,c,alpha,beta,gamma,wavelength,ThisPlane);
						if (Th < ThetaMax){
							ThetaOut[nPlanesFilled] = Th;
							PlanesOut[nPlanesFilled][0] = ThisPlane[0];
							PlanesOut[nPlanesFilled][1] = ThisPlane[1];
							PlanesOut[nPlanesFilled][2] = ThisPlane[2];
							nPlanesFilled++;
						}
					}
				}
			}
		}
	}
	*TotPlanes = nPlanesFilled;
}

void HKLsThetaHex(double a, double b, double c, double alpha, double beta,
	double gamma, double wavelength, double ThetaMax, int PlanesOut[14000][3],
	double ThetaOut[14000], int *TotPlanes)
{
	int hkl[3];
	double Th;
	int nPlanesFilled = 0;
	hkl[0] = 0;
	hkl[1] = 1;
	hkl[2] = 2;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = -1;
	hkl[2] = 2;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = 1;
	hkl[2] = -2;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = -1;
	hkl[2] = -2;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 0;
	hkl[2] = 2;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = 0;
	hkl[2] = 2;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 0;
	hkl[2] = -2;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = 0;
	hkl[2] = -2;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 0;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = 0;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 0;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = 0;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = 1;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = 1;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = -1;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = -1;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 1;
	hkl[2] = 0;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = -1;
	hkl[2] = 0;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 1;
	hkl[2] = 3;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = -1;
	hkl[2] = 3;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 1;
	hkl[2] = -3;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = -1;
	hkl[2] = -3;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = 2;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = -2;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = 2;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = -2;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 2;
	hkl[1] = 0;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -2;
	hkl[1] = 0;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 2;
	hkl[1] = 0;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -2;
	hkl[1] = 0;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 1;
	hkl[2] = 6;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = -1;
	hkl[2] = 6;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 1;
	hkl[2] = -6;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = -1;
	hkl[2] = -6;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = 1;
	hkl[2] = 8;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = -1;
	hkl[2] = 8;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = 1;
	hkl[2] = -8;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = -1;
	hkl[2] = -8;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 0;
	hkl[2] = 8;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = 0;
	hkl[2] = 8;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 0;
	hkl[2] = -8;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = 0;
	hkl[2] = -8;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 2;
	hkl[1] = 1;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -2;
	hkl[1] = -1;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 2;
	hkl[1] = 1;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -2;
	hkl[1] = -1;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 2;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = -2;
	hkl[2] = 4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 2;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = -2;
	hkl[2] = -4;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 3;
	hkl[1] = 0;
	hkl[2] = 0;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -3;
	hkl[1] = 0;
	hkl[2] = 0;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = 3;
	hkl[2] = 0;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = -3;
	hkl[2] = 0;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = 1;
	hkl[2] = 10;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = -1;
	hkl[2] = 10;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = 1;
	hkl[2] = -10;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 0;
	hkl[1] = -1;
	hkl[2] = -10;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 0;
	hkl[2] = 10;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = 0;
	hkl[2] = 10;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 0;
	hkl[2] = -10;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = 0;
	hkl[2] = -10;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 1;
	hkl[2] = 9;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = -1;
	hkl[2] = 9;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = 1;
	hkl[1] = 1;
	hkl[2] = -9;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	hkl[0] = -1;
	hkl[1] = -1;
	hkl[2] = -9;
	Th = Theta(a,b,c,alpha,beta,gamma,wavelength,hkl);
	if (Th < ThetaMax){
		ThetaOut[nPlanesFilled] = Th;
		PlanesOut[nPlanesFilled][0] = hkl[0];
		PlanesOut[nPlanesFilled][1] = hkl[1];
		PlanesOut[nPlanesFilled][2] = hkl[2];
		nPlanesFilled++;
	}
	*TotPlanes = nPlanesFilled;
}

static int cmpfunc (const void * a, const void *b){
	if (*(double *)a > *(double *)b) return 1;
	else if (*(double *)a < *(double *)b) return -1;
	else return 0;
}

int GenerateRingInfo(int SpaceGroup,double a,double b, double c,
	double alpha, double beta, double gamma, double wavelength,
	double TthetaMax, double ThetasSorted[5000],
	int HKLs[5000][4], int *NPs)
{
	int i,j;
	double ThetaMax = TthetaMax/2;
	int PlanesOut[14000][3];
	double ThetaOut[14000];
	int TotPlanes;
	if (SpaceGroup == 167){
		HKLsThetaHex(a,b,c,alpha,beta,gamma,wavelength,ThetaMax,PlanesOut,ThetaOut,&TotPlanes);
	}else{
		GenerateHKLs(SpaceGroup,a,b,c,alpha,beta,gamma,wavelength,ThetaMax,PlanesOut,ThetaOut,&TotPlanes);
	}
	double ThetasAccPlanes[TotPlanes];
	for (i=0;i<TotPlanes;i++) ThetasAccPlanes[i] = ThetaOut[i];
	if (TotPlanes > 5000){
		printf("Total number of planes exceeds 5000. Please reduce MaxTtheta.");
		return 0;
	}
	qsort(ThetaOut, TotPlanes, sizeof(double), cmpfunc);
	double UniqueThetas[5000], ThisTheta;
	int NrUniqueThetas=1,ThetaUnique;
	UniqueThetas[0] = ThetaOut[0];
	for (i=1;i<TotPlanes;i++){
		ThisTheta = ThetaOut[i];
		ThetaUnique = 1;
		for (j=0;j<NrUniqueThetas;j++){
			if ((int)(UniqueThetas[j]*10000) == (int)(ThisTheta*10000)){
				ThetaUnique=0;
				break;
			}
		}
		if (ThetaUnique == 1){
			UniqueThetas[NrUniqueThetas] = ThisTheta;
			NrUniqueThetas++;
		}
	}
	double ThetaComp;
	int NrFilled = 0;
	for (i=0;i<NrUniqueThetas;i++){
		ThetaComp = UniqueThetas[i];
		for (j=0;j<TotPlanes;j++){
			if ((int)(ThetaComp*10000) == (int)(ThetasAccPlanes[j]*10000)){
				HKLs[NrFilled][0] = PlanesOut[j][0];
				HKLs[NrFilled][1] = PlanesOut[j][1];
				HKLs[NrFilled][2] = PlanesOut[j][2];
				HKLs[NrFilled][3] = i+1;
				ThetasSorted[NrFilled] = ThetaComp;
				NrFilled++;
			}
		}
	}
	
	*NPs = TotPlanes;
	return 1;
}
