#include "Constants.h"

int bitScanForward(uint64 bb) {
   const uint64 debruijn64 = uint64(0x03f79d71b4cb0a89);
   return index64[((bb & -bb) * debruijn64) >> 58];
}