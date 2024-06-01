#ifndef BOTS_CPP
#define BOTS_CPP

#include "Bots.h"

RandomBot::RandomBot(){
    rng = std::make_unique<RandomNumberGenerator>(48756);
}

Move RandomBot::makeMove(Position &position, MoveGenerator &moveGenerator) {
    moveGenerator.generateLegalMoves(position);
    int randomMove = rng->generateRandomU64() % moveGenerator.legalMovesCount;
    for (int i = 0; i < moveGenerator.pseudoLegalMovesCount; i++) {
        if (moveGenerator.legalMove[i]) {
            if (randomMove == 0) return moveGenerator.pseudoLegalMoves[i];
            randomMove--;
        }
    }
    return {0, 0, NORMAL, NOPIECE, NOPIECE};
}

MaterialDepthOneBot::MaterialDepthOneBot(){
    rng = std::make_unique<RandomNumberGenerator>(48756);
}

Move MaterialDepthOneBot::makeMove(Position &position, MoveGenerator &moveGenerator) {
    moveGenerator.generateLegalMoves(position);
    int bestScore = 1000000;
    Move bestMove = {0, 0, NORMAL, NOPIECE, NOPIECE};
    for (int i = 0; i < moveGenerator.pseudoLegalMovesCount; i++) {
        if (moveGenerator.legalMove[i]) {
            // U8 castlingRights, U8 prevEnPassantTargetSquare, U8 prevHalfmoveClock, U8 prevFullmoveCounter
            U8 positonInfo[4] = {position.getCastlingAbility(), position.getEnPassantTarget(), position.getHalfmoveClock(), position.getFullmoveCounter()};
            position.makeMove(moveGenerator.pseudoLegalMoves[i]);
            int score = position.getMaterialScore();
            position.unmakeMove(moveGenerator.pseudoLegalMoves[i], positonInfo[0], positonInfo[1], positonInfo[2], positonInfo[3]);
            if (score < bestScore) {
                bestScore = score;
                bestMove = moveGenerator.pseudoLegalMoves[i];
            }
        }
    }
    return bestMove;
}

#endif