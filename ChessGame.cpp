#ifndef CHESS_GAME_CPP
#define CHESS_GAME_CPP

#include "ChessGame.h"

ChessGame::ChessGame() {
    position = Position();
    position.parseFen(STARTING_POSITION_FEN);
    moveGenerator = MoveGenerator();
    movesCount = moveGenerator.generateMoves(position);
    currMoveIndex = 0;
}

Piece ChessGame::getPieceAt(int square) {
    return position.getPieceAt(square);
}

int ChessGame::isLegalMove(int from, int to) {
    for (int i = 0; i < movesCount; i++) {
        if (moveGenerator.legalMoves[i].from == from && moveGenerator.legalMoves[i].to == to) {
            // If I make the move and my king is attacked, it's not a legal move
            makeMove(i, true);
            Piece king = position.getCurrentPlayer() == WHITE ? BLACK_KING : WHITE_KING;
            U8 kingSquare = bitScanForward(position.getOccupiedSquares(king));
            if (moveGenerator.isSquareAttackedByColor(position, kingSquare, position.getCurrentPlayer())){
                unmakeMove(i);
                return -1;
            }
            unmakeMove(i);
            return i;
        }
    }
    return -1;
}

void ChessGame::makeMove(int moveIndex, bool checkingLegality) {
    moveList[currMoveIndex] = moveGenerator.legalMoves[moveIndex];
    castlingAbility[currMoveIndex] = position.getCastlingAbility();
    enPassantSquares[currMoveIndex] = position.getEnPassantTarget();
    halfmoveClock[currMoveIndex] = position.getHalfmoveClock();
    fullmoveCounter[currMoveIndex] = position.getFullmoveCounter();
    position.makeMove(moveGenerator.legalMoves[moveIndex]);
    currMoveIndex++;
    if (!checkingLegality)
        movesCount = moveGenerator.generateMoves(position);
}

void ChessGame::unmakeMove(int moveIndex) {
    currMoveIndex--;
    position.unmakeMove(moveGenerator.legalMoves[moveIndex],
                        castlingAbility[currMoveIndex],
                        enPassantSquares[currMoveIndex],
                        halfmoveClock[currMoveIndex],
                        fullmoveCounter[currMoveIndex]);
}

U64 ChessGame::getMovesFrom(int square) {
    U64 to = 0ULL;
    for (int i = 0; i < movesCount; i++) {
        if (moveGenerator.legalMoves[i].from == square) {
            if (isLegalMove(square, moveGenerator.legalMoves[i].to) != -1)
                to |= setBit(0ULL, moveGenerator.legalMoves[i].to);
        }
    }
    return to;
}

#endif