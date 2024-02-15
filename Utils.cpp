#include "Utils.h"

int bitScanForward(uint64 bb) {
   const uint64 debruijn64 = uint64(0x03f79d71b4cb0a89);
   return index64[((bb & -bb) * debruijn64) >> 58];
}

uint64 soutOne (uint64 b) { return b >> 8; }
uint64 nortOne (uint64 b) { return b << 8; }
uint64 eastOne (uint64 b) { return (b << 1) & notAFile; }
uint64 noEaOne (uint64 b) { return (b << 9) & notAFile; }
uint64 soEaOne (uint64 b) { return (b >> 7) & notAFile; }
uint64 westOne (uint64 b) { return (b >> 1) & notHFile; }
uint64 soWeOne (uint64 b) { return (b >> 9) & notHFile; }
uint64 noWeOne (uint64 b) { return (b << 7) & notHFile; }