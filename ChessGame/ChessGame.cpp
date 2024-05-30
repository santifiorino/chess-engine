#ifndef CHESS_GAME_CPP
#define CHESS_GAME_CPP

#include "ChessGame.h"
#include <iostream>

ChessGame::ChessGame() {
    isGameOver = false;
    currMoveIndex = 0;
    position = Position();
    position.parseFen(STARTING_POSITION_FEN);
    moveGenerator = MoveGenerator();
    moveGenerator.generateLegalMoves(position);
}

Piece ChessGame::getPieceAt(int square) {
    return position.getPieceAt(square);
}

bool ChessGame::makeMove(U8 from, U8 to) {
    int moveIndex = moveGenerator.getMoveIndex(position, from, to);
    if (moveIndex == -1) return false;
    Move move = moveGenerator.pseudoLegalMoves[moveIndex];
    // Save position data and make the move
    moveList[currMoveIndex] = move;
    castlingAbility[currMoveIndex] = position.getCastlingAbility();
    enPassantSquares[currMoveIndex] = position.getEnPassantTarget();
    halfmoveClock[currMoveIndex] = position.getHalfmoveClock();
    fullmoveCounter[currMoveIndex] = position.getFullmoveCounter();
    position.makeMove(move);
    currMoveIndex++;

    if (position.halfmoveClockIsFifty()) {
        std::cout << "Draw by fifty-move rule!" << std::endl;
        isGameOver = true;
    }

    // Check if the game is over
    moveGenerator.generateLegalMoves(position);
    if (moveGenerator.legalMovesCount == 0) {
        Color enemyPlayer = position.getCurrentPlayer() == WHITE ? BLACK : WHITE;
        Piece king = enemyPlayer == WHITE ? BLACK_KING : WHITE_KING;
        U8 kingSquare = bitScanForward(position.getOccupiedSquares(king));
        if (moveGenerator.isSquareAttackedByColor(position, kingSquare, enemyPlayer)) {
            std::cout << "Checkmate!" << std::endl;
        } else {
            std::cout << "Stalemate!" << std::endl;
        }
        isGameOver = true;
    }
    return true;
}

bool ChessGame::unmakeMove(U8 from, U8 to) {
    int moveIndex = moveGenerator.getMoveIndex(position, from, to);
    if (moveIndex == -1) return false;
    Move move = moveGenerator.pseudoLegalMoves[moveIndex];
    currMoveIndex--;
    position.unmakeMove(move,
                        castlingAbility[currMoveIndex],
                        enPassantSquares[currMoveIndex],
                        halfmoveClock[currMoveIndex],
                        fullmoveCounter[currMoveIndex]);
    return true;
}

U64 ChessGame::getMovesFrom(int square) {
    return moveGenerator.getMovesFrom(position, square);
}

Color ChessGame::getCurrentPlayer() {
    return position.getCurrentPlayer();
}

void ChessGame::makeAIMove() {
    moveGenerator.generateLegalMoves(position);
    // select a random move
    int randomMoveIndex = rand() % moveGenerator.legalMovesCount;
    for (int i = 0; i < moveGenerator.pseudoLegalMovesCount; i++) {
        if (moveGenerator.legalMove[i]) {
            randomMoveIndex--;
            if (randomMoveIndex == -1) {
                makeMove(moveGenerator.pseudoLegalMoves[i].from, moveGenerator.pseudoLegalMoves[i].to);
                break;
            }
        }
    }
}

#endif