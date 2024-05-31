#ifndef POSITIONHASHING_CPP
#define POSITIONHASHING_CPP

#include "Position.h"
#include <iostream>

void Position::initPositionHash() {
    positionHash = 0ULL;
    for (int i = 0; i < 64; i++) {
        if (getPieceAt(i) != NOPIECE) {
            positionHash ^= zobristRandomNumbers[i * 12 + getPieceAt(i)];
        }
    }
    if (whiteCanKingsideCastle()) positionHash ^= zobristRandomNumbers[768];
    if (whiteCanQueensideCastle()) positionHash ^= zobristRandomNumbers[769];
    if (blackCanKingsideCastle()) positionHash ^= zobristRandomNumbers[770];
    if (blackCanQueensideCastle()) positionHash ^= zobristRandomNumbers[771];
    if (enPassantTargetSquare != 64) {
        int enPassantFile = enPassantTargetSquare % 8;
        positionHash ^= zobristRandomNumbers[772 + enPassantFile];
    }
    if (getCurrentPlayer() == BLACK) positionHash ^= zobristRandomNumbers[780];
}

#endif