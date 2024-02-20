#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "../MoveGenerator/MoveGenerator.h"

class ChessGame {
public:
    ChessGame();
    Piece getPieceAt(int square);
    bool makeMove(U8 from, U8 to, bool checkCheckmate=true);
    void unmakeMove(U8 from, U8 to);
    U64 getMovesFrom(int from);

private:
    Position position;
    MoveGenerator moveGenerator;
    bool legalMove[218];
    int movesCount;

    int generateLegalMoves();
    bool isMoveLegal(U8 from, U8 to);
    int getMoveIndex(U8 from, U8 to);

    int currMoveIndex;
    Move moveList[1024];
    U8 castlingAbility[1024];
    U8 enPassantSquares[1024];
    U8 halfmoveClock[1024];
    U8 fullmoveCounter[1024];
};

#endif