#ifndef __TIA__
#define __TIA__
#include <stdio.h>
#include "SDL2/SDL.h"
#include "memory.h"
//Tia
static unsigned int SCR_WIDTH = 228;
static unsigned int SCR_HEIGHT = 262;
static unsigned int scanline = 0;
static unsigned int step = 0;
static BYTE rawBgColor = 0;
static bool isCpuStopped = false;
//SDL
static SDL_Window* win = NULL;
static SDL_Renderer* ren = NULL;

void initGraph(); //Init graphics
void tiaReset();	//Init TIA
void tiaStep();		//Tia step
void close();		//Close tia session
bool getCpuState(); //Get isCpuStopped

#endif