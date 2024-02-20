#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "MoveGenerator/MoveGenerator.h"

class ChessGame {
public:
    ChessGame();
    Piece getPieceAt(int square);
    int moveIndex(U8 from, U8 to);
    void makeMove(int moveIndex, bool checkingLegality = false);
    void unmakeMove(int moveIndex);
    U64 getMovesFrom(int from);

private:
    Position position;
    MoveGenerator moveGenerator;
    int legalMovesIndex[218];
    int movesCount;

    int generateLegalMoves();
    int checkMoveLegality(int index);

    int currMoveIndex;
    Move moveList[1024];
    U8 castlingAbility[1024];
    U8 enPassantSquares[1024];
    U8 halfmoveClock[1024];
    U8 fullmoveCounter[1024];
};

#endif