#ifndef CHESS_GAME_CPP
#define CHESS_GAME_CPP

#include "ChessGame.h"
#include <iostream>

ChessGame::ChessGame() {
    position = Position();
    position.parseFen(STARTING_POSITION_FEN);
    moveGenerator = MoveGenerator();
    generateLegalMoves();
    currMoveIndex = 0;
}

int ChessGame::generateLegalMoves() {
    movesCount = moveGenerator.generateMoves(position);
    int legalMovesCount = 0;
    for (int i = 0; i < movesCount; i++) {
        U8 from = moveGenerator.pseudoLegalMoves[i].from;
        U8 to = moveGenerator.pseudoLegalMoves[i].to;
        legalMovesIndex[i] = checkMoveLegality(i);
        if (legalMovesIndex[i] != -1){
            legalMovesCount++;
            std::cout << "legal move from " << (int)from << " to " << (int)to << std::endl;
        } 
    }
    return legalMovesCount;
}

int ChessGame::moveIndex(U8 from, U8 to) {
    for (int i = 0; i < movesCount; i++) {
        U8 fromGen = moveGenerator.pseudoLegalMoves[i].from;
        U8 toGen = moveGenerator.pseudoLegalMoves[i].to;
        if (fromGen == from && toGen == to) {
            return legalMovesIndex[i];
        }
    }
    return -1;
}

Piece ChessGame::getPieceAt(int square) {
    return position.getPieceAt(square);
}

int ChessGame::checkMoveLegality(int index) {
    // If I make the move and my king is attacked, it's not a legal move
    Piece king = position.getCurrentPlayer() == WHITE ? WHITE_KING : BLACK_KING;
    makeMove(index, true);
    U8 kingSquare = bitScanForward(position.getOccupiedSquares(king));
    if (moveGenerator.isSquareAttackedByColor(position, kingSquare, position.getCurrentPlayer())){
        unmakeMove(index);
        return -1;
    }
    unmakeMove(index);
    return index;
}

void ChessGame::makeMove(int moveIndex, bool checkingLegality) {
    moveList[currMoveIndex] = moveGenerator.pseudoLegalMoves[moveIndex];
    castlingAbility[currMoveIndex] = position.getCastlingAbility();
    enPassantSquares[currMoveIndex] = position.getEnPassantTarget();
    halfmoveClock[currMoveIndex] = position.getHalfmoveClock();
    fullmoveCounter[currMoveIndex] = position.getFullmoveCounter();
    position.makeMove(moveGenerator.pseudoLegalMoves[moveIndex]);
    currMoveIndex++;
    if (!checkingLegality) {
        movesCount = generateLegalMoves();
        std::cout << "Moves count: " << movesCount << std::endl;
        if (movesCount == 0) {
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
}

void ChessGame::unmakeMove(int moveIndex) {
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
            if (legalMovesIndex[i] != -1)
                to |= setBit(0ULL, moveGenerator.pseudoLegalMoves[i].to);
        }
    }
    return to;
}

#endif