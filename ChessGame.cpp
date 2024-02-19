#ifndef CHESS_GAME_CPP
#define CHESS_GAME_CPP

#include "ChessGame.h"

ChessGame::ChessGame() {
    position = Position();
    position.parseFen(STARTING_POSITION_FEN);
    moveGenerator = MoveGenerator();
    movesCount = moveGenerator.generateMoves(position);
}

Piece ChessGame::getPieceAt(int square) {
    return position.getPieceAt(square);
}

int ChessGame::isLegalMove(int from, int to) {
    for (int i = 0; i < movesCount; i++) {
        if (moveGenerator.legalMoves[i].from == from && moveGenerator.legalMoves[i].to == to) {
            return i;
        }
    }
    return -1;
}

void ChessGame::makeMove(int moveIndex) {
    position.makeMove(moveGenerator.legalMoves[moveIndex]);
    movesCount = moveGenerator.generateMoves(position);
}

U64 ChessGame::getMovesFrom(int square) {
    U64 to = 0ULL;
    for (int i = 0; i < movesCount; i++) {
        if (moveGenerator.legalMoves[i].from == square) {
            to |= setBit(0ULL, moveGenerator.legalMoves[i].to);
        }
    }
    return to;
}

#endif