#ifndef POSITION_CPP
#define POSITION_CPP

#include "Position.h"

Position::Position() {
    initializeBoards();
    castlingAbility = 0;
    enPassantTargetSquare = NOSQUARE;
    halfmoveClock = 0;
    fullmoveCounter = 1;

    RandomNumberGenerator rng = RandomNumberGenerator(1804289383);

    for (int i = 0; i < 781; i++) {
        zobristRandomNumbers[i] = rng.generateRandomU64();
    }
}

void Position::initializeBoards() {
    for (int i = 0; i < 12; i++) {
        bitboards[i] = 0;
    }
}

#endif