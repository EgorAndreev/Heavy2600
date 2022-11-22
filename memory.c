#include "memory.h"
#include <conio.h>
#include <stdio.h>

BYTE internalMemory[MEM_MASK] = { 0 };

void initMemory(char* romPath)
{
    FILE* romFile = fopen(romPath, "rb");
    if (romFile == NULL) {
        perror("fopen() ");
        return;
    }
    unsigned char c;
    int i = 0;
    while (!feof(romFile)) {
        BYTE c = fgetc(romFile);
        internalMemory[4096 + i] = c;
        i++;
    }
    fclose(romFile);

}

void writeByte(WORD addr, BYTE data)
{
    internalMemory[addr & MEM_MASK] = data;
}

WORD readWord(WORD addr)
{
    BYTE lo = internalMemory[addr & MEM_MASK];
    BYTE hi = internalMemory[(addr +1)& MEM_MASK];
    return (hi << 8) | lo;
}

bool isPageCrossed(WORD newAddr, WORD oldAddr)
{
    if(HIGH(newAddr) > HIGH(oldAddr)) {
        return true;
    }
    else {
        return false;
    }
}
