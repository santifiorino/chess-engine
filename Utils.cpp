#include "Utils.h"

U64 soutOne (U64 b) { return b >> 8; }
U64 nortOne (U64 b) { return b << 8; }
U64 eastOne (U64 b) { return (b << 1) & notAFile; }
U64 noEaOne (U64 b) { return (b << 9) & notAFile; }
U64 soEaOne (U64 b) { return (b >> 7) & notAFile; }
U64 westOne (U64 b) { return (b >> 1) & notHFile; }
U64 soWeOne (U64 b) { return (b >> 9) & notHFile; }
U64 noWeOne (U64 b) { return (b << 7) & notHFile; }

int bitScanForward(U64 bb) {
   const U64 debruijn64 = U64(0x03f79d71b4cb0a89);
   return index64[((bb & -bb) * debruijn64) >> 58];
}

int countOnes(U64 b) {
   int r;
   for(r = 0; b; r++, b &= b - 1);
   return r;
}

unsigned int RandomNumberGenerator::generateRandomU32() {
   seed ^= (seed << 13);
   seed ^= (seed >> 17);
   seed ^= (seed << 5);
   return seed;
}

U64 RandomNumberGenerator::generateRandomU64() {
   U64 u1, u2, u3, u4;
   u1 = generateRandomU32() & 0xFFFF;
   u2 = generateRandomU32() & 0xFFFF;
   u3 = generateRandomU32() & 0xFFFF;
   u4 = generateRandomU32() & 0xFFFF;
   return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

U64 RandomNumberGenerator::generateRandomU64FewBits() {
   return generateRandomU64() & generateRandomU64() & generateRandomU64();
}