#ifndef __PALETTE__
#define __PALETTE__
#include "memory.h"
typedef struct RGB_color {
	int r;
	int g;
	int b;
}RGB_color;
extern RGB_color NTSCPalette[16][8];
#endif