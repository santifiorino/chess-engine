#ifndef CHESS_GAME_CPP
#define CHESS_GAME_CPP

#include "ChessGame.h"
#include <iostream>

ChessGame::ChessGame() {
    currMoveIndex = 0;
    position = Position();
    position.parseFen(STARTING_POSITION_FEN);
    moveGenerator = MoveGenerator();
    generateLegalMoves();
}

int ChessGame::generateLegalMoves() {
    movesCount = moveGenerator.generateMoves(position);
    int legalMovesCount = 0;
    for (int i = 0; i < movesCount; i++) {
        U8 from = moveGenerator.pseudoLegalMoves[i].from;
        U8 to = moveGenerator.pseudoLegalMoves[i].to;
        legalMove[i] = isMoveLegal(from, to);
        if (legalMove[i]) legalMovesCount++;
    }
    return legalMovesCount;
}

Piece ChessGame::getPieceAt(int square) {
    return position.getPieceAt(square);
}

bool ChessGame::isMoveLegal(U8 from, U8 to) {
    // If I make the move and my king is attacked, it's not a legal move
    Piece king = position.getCurrentPlayer() == WHITE ? WHITE_KING : BLACK_KING;
    makeMove(from, to, false);
    U8 kingSquare = bitScanForward(position.getOccupiedSquares(king));
    if (moveGenerator.isSquareAttackedByColor(position, kingSquare, position.getCurrentPlayer())){
        unmakeMove(from, to);
        return false;
    }
    unmakeMove(from, to);
    return true;
}

bool ChessGame::makeMove(U8 from, U8 to, bool checkCheckmate) {
    int moveIndex = getMoveIndex(from, to);
    if (moveIndex == -1) return false;
    if (checkCheckmate)
        if (!legalMove[moveIndex]) return false;
    moveList[currMoveIndex] = moveGenerator.pseudoLegalMoves[moveIndex];
    castlingAbility[currMoveIndex] = position.getCastlingAbility();
    enPassantSquares[currMoveIndex] = position.getEnPassantTarget();
    halfmoveClock[currMoveIndex] = position.getHalfmoveClock();
    fullmoveCounter[currMoveIndex] = position.getFullmoveCounter();
    position.makeMove(moveGenerator.pseudoLegalMoves[moveIndex]);
    currMoveIndex++;
    if (checkCheckmate) {
        int legalMoves = generateLegalMoves();
        if (legalMoves == 0) {
            Color enemyPlayer = position.getCurrentPlayer() == WHITE ? BLACK : WHITE;
            Piece king = enemyPlayer == WHITE ? BLACK_KING : WHITE_KING;
            U8 kingSquare = bitScanForward(position.getOccupiedSquares(king));
            if (moveGenerator.isSquareAttackedByColor(position, kingSquare, enemyPlayer)) {
                std::cout << "Checkmate!" << std::endl;
            } else {
                std::cout << "Stalemate!" << std::endl;
            }
            exit(0);
        }
    }
    return true;
}

void ChessGame::unmakeMove(U8 from, U8 to) {
    int moveIndex = getMoveIndex(from, to);
    currMoveIndex--;
    position.unmakeMove(moveGenerator.pseudoLegalMoves[moveIndex],
                        castlingAbility[currMoveIndex],
                        enPassantSquares[currMoveIndex],
                        halfmoveClock[currMoveIndex],
                        fullmoveCounter[currMoveIndex]);
}

U64 ChessGame::getMovesFrom(int square) {
    U64 to = 0ULL;
    for (int i = 0; i < movesCount; i++) {
        if (moveGenerator.pseudoLegalMoves[i].from == square) {
            if (legalMove[i])
                to |= setBit(0ULL, moveGenerator.pseudoLegalMoves[i].to);
        }
    }
    return to;
}

int ChessGame::getMoveIndex(U8 from, U8 to) {
    for (int i = 0; i < movesCount; i++) {
        if (moveGenerator.pseudoLegalMoves[i].from == from && moveGenerator.pseudoLegalMoves[i].to == to) {
            return i;
        }
    }
    return -1;
}

#endif