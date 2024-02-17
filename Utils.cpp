#include "Utils.h"

int bitScanForward(U64 bb) {
   const U64 debruijn64 = U64(0x03f79d71b4cb0a89);
   return index64[((bb & -bb) * debruijn64) >> 58];
}

U64 soutOne (U64 b) { return b >> 8; }
U64 nortOne (U64 b) { return b << 8; }
U64 eastOne (U64 b) { return (b << 1) & notAFile; }
U64 noEaOne (U64 b) { return (b << 9) & notAFile; }
U64 soEaOne (U64 b) { return (b >> 7) & notAFile; }
U64 westOne (U64 b) { return (b >> 1) & notHFile; }
U64 soWeOne (U64 b) { return (b >> 9) & notHFile; }
U64 noWeOne (U64 b) { return (b << 7) & notHFile; }