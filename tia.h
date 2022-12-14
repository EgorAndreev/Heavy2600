#ifndef __TIA__
#define __TIA__
#include <stdio.h>
#include "SDL2/SDL.h"
#include "memory.h"
extern void logPos();
//Tia
static unsigned int SCR_WIDTH = 228;
static unsigned int SCR_HEIGHT = 262;
static unsigned int scanline = 0;
static unsigned int step = 0;
static BYTE rawBgColor = 0;
static bool vsync = false;
//SDL
static SDL_Window* win = NULL;
static SDL_Renderer* ren = NULL;

void initGraph(); //Init graphics
void tiaReset();	//Init TIA
void tiaStep(bool* wsynced);		//Tia step
void close();		//Close tia session
#endif