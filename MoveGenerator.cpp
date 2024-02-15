#include "MoveGenerator.h"
#include "Position.h"
#include "Constants.h"

int MoveGenerator::generateMoves(Position &position) {
    int i = 0;
    if (position.getCurrentPlayer() == WHITE) {
        uint64 whitePawns = position.getBitboard(WHITE_PAWN);
        uint64 empty = position.getEmptySquares();
        generatePawnPushes(whitePawnsAbleToPush(whitePawns, empty), i, WHITE, false);
        generatePawnPushes(whitePawnsAbleToDoublePush(whitePawns, empty), i, WHITE, true);
    } else {
        uint64 blackPawns = position.getBitboard(BLACK_PAWN);
        uint64 empty = position.getEmptySquares();
        generatePawnPushes(blackPawnsAbleToPush(blackPawns, empty), i, BLACK, false);
        generatePawnPushes(blackPawnsAbleToDoublePush(blackPawns, empty), i, BLACK, true);
    }
    return i;
}

uint64 MoveGenerator::whitePawnsAbleToPush(uint64 whitePawns, uint64 empty) {
    return soutOne(empty) & whitePawns;
}

uint64 MoveGenerator::blackPawnsAbleToPush(uint64 blackPawns, uint64 empty) {
    return nortOne(empty) & blackPawns;
}

uint64 MoveGenerator::whitePawnsAbleToDoublePush(uint64 whitePawns, uint64 empty) {
    uint64 emptyRank3 = soutOne(empty & RANK_4) & empty;
    return whitePawnsAbleToPush(whitePawns, emptyRank3);
}

uint64 MoveGenerator::blackPawnsAbleToDoublePush(uint64 blackPawns, uint64 empty) {
    uint64 emptyRank6 = nortOne(empty & RANK_5) & empty;
    return blackPawnsAbleToPush(blackPawns, emptyRank6);
}

void MoveGenerator::generatePawnPushes(uint64 pushes, int& i, Color color, bool isDoublePush) {
    uint64 serializedPushes[8];
    for (int j = 0; j < 8; j++) {
        serializedPushes[j] = 0;
    }
    int j = 0;
    while (pushes) {
        serializedPushes[j] = bitScanForward(pushes);
        pushes &= pushes - 1;
        j++;
    }
    j = 0;
    for (int k = 0; k < 8; k++) {
        if (serializedPushes[k] == 0) break;
        uint8 from = serializedPushes[k];
        uint8 to = from + (isDoublePush ? 16 : 8) * (color == WHITE ? 1 : -1);
        Move move = {from, to, NORMAL, EMPTY, EMPTY};
        legalMoves[i] = move;
        i++;
    }
}

