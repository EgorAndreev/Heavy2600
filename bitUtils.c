#include "bitUtils.h"

BYTE SetBit(BYTE byte, unsigned index)
{
    return byte | (1UL << index);
}
BYTE EraseBit(BYTE byte, unsigned index) {
    return byte & ~(1UL << index);
}

bool GetBit(BYTE byte, unsigned index)
{
    return (byte >> index) & 1U;
}
