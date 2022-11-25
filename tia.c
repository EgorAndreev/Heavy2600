#include "tia.h"
#include "palette.h"
#include "6502.h"
void logPos()
{
    printf("STEP: %d\t", step);
    printf("SCANLINE: %d\n", scanline);
}
void initGraph() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("%s - error init SDL2!!!", SDL_GetError());
        return;
    }
    win = SDL_CreateWindow("Hello World!", 500, 500, SCR_WIDTH*5, SCR_HEIGHT*3, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("%s - error creat SDL2 window!!!", SDL_GetError());
        return;
    }
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        printf("%s - error creat SDL2 renderer!!!", SDL_GetError());
        return;
    }
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderSetScale(ren, 5.f, 3.f);
}
void tiaReset()
{

}

void tiaStep(bool* wsynced)
{
    //WSYNC
    if (*wsynced) {
        *wsynced = false;
        SDL_RenderDrawLine(ren, step, scanline, SCR_WIDTH, scanline);
        scanline += 1;
        step = 0;
        return;
    }
    if (readByte(0) != 0 && vsync == false) { 
        vsync = true;
        scanline = step = 0;
    }
    if (readByte(0) == 0) {
        vsync = false;
    }
    //COLUBK changed
    if (rawBgColor != internalMemory[0x9] && *wsynced != true) {
        rawBgColor = internalMemory[0x9];
        int t = rawBgColor / 16;
        int o = rawBgColor % 16;
        RGB_color rgb = NTSCPalette[t][(o/2)];
        SDL_SetRenderDrawColor(ren, rgb.r, rgb.g, rgb.b, 255);
    }
    
    //New scanline
    if (step >= SCR_WIDTH) {
        step = 0;
        scanline += 1;
        SDL_RenderPresent(ren);
        return;
    }
    //New frame
    if (scanline >= SCR_HEIGHT) {
        scanline = step = 0;
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderPresent(ren);
        SDL_Delay(16);
        return;
    }
    
    step += 3;
    if (step >= 68) {
        SDL_RenderDrawPoint(ren, step - 2, scanline);
        SDL_RenderDrawPoint(ren, step - 1, scanline);
        SDL_RenderDrawPoint(ren, step, scanline);
    }

    SDL_Event event;
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        return;
}

void close()
{
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
