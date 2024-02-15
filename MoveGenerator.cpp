#include "MoveGenerator.h"
#include "Position.h"
#include "Constants.h"

int MoveGenerator::generateMoves(Position &position) {
    uint64 sourceWhitePawns = whitePawnsAbleToPush(position.getBitboard(WHITE_PAWN), position.getEmptySquares());
    uint8 pawnSources[8];
    for (int i = 0; i < 8; i++) {
        pawnSources[i] = 0;
    }
    int i = 0;
    while (sourceWhitePawns) {
        pawnSources[i] = bitScanForward(sourceWhitePawns);
        sourceWhitePawns &= sourceWhitePawns - 1;
        i++;
    }
    for (int j = 0; j < 8; j++) {
        if (pawnSources[j] == 0) break;
        uint8 from = pawnSources[j];
        uint8 to = from + 8;
        Move move = {from, to, NORMAL, EMPTY, EMPTY};
        legalMoves[j] = move;
    }
    return i;
}

uint64 MoveGenerator::whitePawnsAbleToPush(uint64 whitePawns, uint64 empty) {
    return soutOne(empty) & whitePawns;
}

