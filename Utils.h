#ifndef UTILS_H
#define UTILS_H

typedef unsigned char U8;
typedef unsigned long long U64;

enum Piece {
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
    BLACK_KING,
    NOPIECE
};

enum PieceType {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NOTYPE
};

enum Color {
    WHITE,
    BLACK
};

enum MoveType {
    NORMAL,
    DOUBLE_PAWN_PUSH,
    CAPTURE,
    EN_PASSANT,
    KINGSIDE_CASTLE,
    QUEENSIDE_CASTLE,
    PROMOTION,
    PROMOTION_CAPTURE
};

struct Move {
    U8 from;
    U8 to;
    MoveType type;
    Piece captured;
    Piece promotion;
};

enum AlgebraicNotation {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NOSQUARE
};

const U64 RANK_1 = 0xFF;
const U64 RANK_2 = 0xFF00;
const U64 RANK_3 = 0xFF0000;
const U64 RANK_4 = 0xFF000000;
const U64 RANK_5 = 0xFF00000000;
const U64 RANK_6 = 0xFF0000000000;
const U64 RANK_7 = 0xFF000000000000;
const U64 RANK_8 = 0xFF00000000000000;

const U64 FILE_A = 0x0101010101010101;
const U64 FILE_B = 0x0202020202020202;
const U64 FILE_C = 0x0404040404040404;
const U64 FILE_D = 0x0808080808080808;
const U64 FILE_E = 0x1010101010101010;
const U64 FILE_F = 0x2020202020202020;
const U64 FILE_G = 0x4040404040404040;
const U64 FILE_H = 0x8080808080808080;

enum Direction {
    NORTH_WEST = 7,
    NORTH = 8,
    NORTH_EAST = 9,
    WEST = -1,
    EAST = 1,
    SOUTH_WEST = -9,
    SOUTH = -8,
    SOUTH_EAST = -7
};

U64 soutOne (U64 b);
U64 nortOne (U64 b);
const U64 notAFile = 0xFEFEFEFEFEFEFEFE;
const U64 notHFile = 0x7F7F7F7F7F7F7F7F;
U64 eastOne (U64 b);
U64 noEaOne (U64 b);
U64 soEaOne (U64 b);
U64 westOne (U64 b);
U64 soWeOne (U64 b);
U64 noWeOne (U64 b);

inline int toBoardIndex(int file, int rank) {
    return file * 8 + rank;
}

inline U64 getBit(U64 bitboard, int i) {
    return (bitboard >> i) & 1ULL;
}

inline U64 setBit(U64 bitboard, int i) {
    return bitboard | (1ULL << i);
}

// FEN parsing helper functions
inline bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

inline int toDigit(char c) {
    return c - '0';
}

const char STARTING_POSITION_FEN[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

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

int bitScanForward(U64 bb);

int countOnes(U64 b);

extern U64 arrBishopMoves[64][512];
extern U64 arrRookMoves[64][4096];

// Pseudorandom number generator
class RandomNumberGenerator {
private:
    unsigned int seed;

public:
    RandomNumberGenerator(unsigned int seed) : seed(seed) {}
    unsigned int generateRandomU32();
    U64 generateRandomU64();
    U64 generateRandomU64FewBits();
};

#endif