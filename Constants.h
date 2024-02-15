#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long long uint64;

enum Piece {
    EMPTY = -1,
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING
};

enum Color {
    WHITE,
    BLACK
};

enum AlgebraicNotation {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8
};


const uint64 RANK_1 = 0xFF;
const uint64 RANK_2 = 0xFF00;
const uint64 RANK_3 = 0xFF0000;
const uint64 RANK_4 = 0xFF000000;
const uint64 RANK_5 = 0xFF00000000;
const uint64 RANK_6 = 0xFF0000000000;
const uint64 RANK_7 = 0xFF000000000000;
const uint64 RANK_8 = 0xFF00000000000000;



inline uint64 nortOne(uint64 b) {
    return b << 8;
}

inline uint64 soutOne(uint64 b) {
    return b >> 8;
}

// file and rank to array index
inline int toBoardIndex(int file, int rank) {
    return file * 8 + rank;
}

// FEN parsing helper functions
inline bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

inline int toDigit(char c) {
    return c - '0';
}

/**
 * bitScanForward
 * @author Martin Läuter (1997)
 *         Charles E. Leiserson
 *         Harald Prokop
 *         Keith H. Randall
 * "Using de Bruijn Sequences to Index a 1 in a Computer Word"
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
const int index64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};

int bitScanForward(uint64 bb);

#endif