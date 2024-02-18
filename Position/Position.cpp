#ifndef POSITION_CPP
#define POSITION_CPP

#include "Position.h"

Position::Position(char* FEN) {
    initializeBoards();
    int i = 0;
    parsePiecePlacement(FEN, i);
    parsePlayer(FEN, i);
    parseCastlingAbility(FEN, i);
    parseEnPassantTarget(FEN, i);
    parseHalfmoveClock(FEN, i);
    parseFullmoveCounter(FEN, i);
}

void Position::initializeBoards() {
    for (int i = 0; i < 12; i++) {
        bitboards[i] = 0;
    }
}

#endif