#include "types.h"
#include <stdbool.h>
#ifndef __MEMORY__
#define __MEMORY__
#ifdef MOS6507MODE
#define MEM_MASK 0x1fff
#elif MOS6502MODE
#define MEM_MASK 0xffff
#else
#define MEM_MASK 0x1fff
#endif // 65xxMODE
#define LOW(x) (x & 0xFF)
#define HIGH(x) ((x >> 8) & 0xFF)
extern BYTE internalMemory[MEM_MASK];
void initMemory(char* romPath);
void writeByte(WORD addr, BYTE data);
WORD readWord(WORD addr);
BYTE readByte(WORD addr);
bool isPageCrossed(WORD newAddr, WORD oldAddr);
#endif