#ifndef __PALETTE__
#define __PALETTE__
#include "memory.h"
typedef struct RGB_color {
	int r;
	int g;
	int b;
}RGB_color;
typedef struct Palette_entity {
	BYTE colorCode;
	RGB_color rgb;
}Palette_entity;
//Simple Dictionary
extern Palette_entity NTSCPalette[128];
extern RGB_color getRgb(code);
extern BYTE getCode(code);
#endif