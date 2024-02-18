#ifndef POSITION_CPP
#define POSITION_CPP

#include "Position.h"

Position::Position() {
    initializeBoards();
    castlingAbility = 0;
    enPassantTargetSquare = NOSQUARE;
    halfmoveClock = 0;
    fullmoveCounter = 0;
}

void Position::initializeBoards() {
    for (int i = 0; i < 12; i++) {
        bitboards[i] = 0;
    }
}

#endif