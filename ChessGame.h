#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "MoveGenerator/MoveGenerator.h"

class ChessGame {
public:
    ChessGame();
    Piece getPieceAt(int square);
    int isLegalMove(int from, int to);
    void makeMove(int moveIndex);
    U64 getMovesFrom(int from);

private:
    Position position;
    MoveGenerator moveGenerator;
    int movesCount;

};

#endif