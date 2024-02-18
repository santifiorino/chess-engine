#ifndef MOVEGENERATORKNIGHTS_CPP
#define MOVEGENERATORKNIGHTS_CPP

#include "MoveGenerator.h"

void MoveGenerator::precalculateKnightMoves() {
    for (int i = 0; i < 64; i++) {
        U64 knight = setBit(0ULL, i);
        U64 l1 = (knight >> 1) & 0x7F7F7F7F7F7F7F7FULL;
        U64 l2 = (knight >> 2) & 0x3F3F3F3F3F3F3F3FULL;
        U64 r1 = (knight << 1) & 0xFEFEFEFEFEFEFEFEULL;
        U64 r2 = (knight << 2) & 0xFCFCFCFCFCFCFCFCULL;
        U64 h1 = l1 | r1;
        U64 h2 = l2 | r2;
        arrKnightMoves[i] = (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
    }
}

void MoveGenerator::generateKnightMoves(Position& position, int& i) {
    U64 knights = position.getFriendlyPieces(KNIGHT);
    U64 friendlyPieces = position.getFriendlyPieces();
    while (knights) {
        U8 from = bitScanForward(knights);
        U64 moves = arrKnightMoves[from] & ~friendlyPieces;
        while (moves) {
            U8 to = bitScanForward(moves);
            addMove(i, from, to, position.getPieceAt(to) == NOPIECE ? NORMAL : CAPTURE, position.getPieceAt(to), NOTYPE);
            moves &= moves - 1;
        }
        knights &= knights - 1;
    }
}

#endif