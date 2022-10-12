#include "tia.h"
#include "palette.h"

bool getCpuState() {
    return isCpuStopped;
}

void initGraph() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("%s - error init SDL2!!!", SDL_GetError());
        return;
    }
    win = SDL_CreateWindow("Hello World!", 500, 500, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("%s - error creat SDL2 window!!!", SDL_GetError());
        return;
    }
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        printf("%s - error creat SDL2 renderer!!!", SDL_GetError());
        return;
    }
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
}
void tiaReset()
{

}

void tiaStep()
{
    //COLUBK changed
    if (rawBgColor != readByte(0x9)) {
        rawBgColor = readByte(0x9);
        int t = rawBgColor / 10;
        int o = rawBgColor % 10;
        RGB_color rgb = NTSCPalette[t][o/2];
        SDL_SetRenderDrawColor(ren, rgb.r, rgb.g, rgb.b, 255);
    }
    //WSYNC
    if (readByte(0x2) != 0) {
        isCpuStopped = true;
    }
    //New scanline
    if (step >= SCR_WIDTH) {
        step = 0;
        scanline += 1;
        isCpuStopped = false;
        writeByte(0x2, 0x0);
    }
    //New frame
    if (scanline >= SCR_HEIGHT) {
        scanline = step = 0;
        isCpuStopped = false;
        writeByte(0x2, 0x0);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderPresent(ren);
        SDL_RenderClear(ren);
        SDL_Delay(16);
    }
    step += 1;
    SDL_RenderDrawPoint(ren, step, scanline);
    step += 1;
    SDL_RenderDrawPoint(ren, step, scanline);
    step += 1;
    SDL_RenderDrawPoint(ren, step, scanline);

   // printf("%#02x\n", readByte(9));
 
    
    /*
    if (step >=68 && step <SCR_WIDTH ) {
        step += 1;
        
    }
    else if(step<68) {
        //HBLANK!
        step += 1;
    }else {
        step = 0;
        if (scanline < 37) {
            //VBLANK
            scanline += 1;
        }
        else if (scanline >= 30) {
            //OVERSCAN
            scanline += 1;
        }
        SDL_RenderDrawPoint(ren, step, scanline);
    }
    
    if (scanline >= SCR_HEIGHT) {
        scanline = step = 0;
        SDL_RenderPresent(ren);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    }
    */
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
