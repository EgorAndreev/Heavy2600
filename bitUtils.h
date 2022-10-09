#include "types.h"
#include <stdbool.h>
#ifndef __BITMASK__
#define __BITMASK__
BYTE SetBit(BYTE byte, unsigned index);
BYTE EraseBit(BYTE byte, unsigned index);
bool GetBit(BYTE byte, unsigned index);
#endif