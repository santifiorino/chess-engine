#ifndef POSITIONACCESSING_CPP
#define POSITIONACCESSING_CPP

#include "Position.h"

Piece Position::getPieceAt(int square) {
    if (bitboards[WHITE_PAWN] & setBit(0ULL, square)) return WHITE_PAWN;
    if (bitboards[WHITE_KNIGHT] & setBit(0ULL, square)) return WHITE_KNIGHT;
    if (bitboards[WHITE_BISHOP] & setBit(0ULL, square)) return WHITE_BISHOP;
    if (bitboards[WHITE_ROOK] & setBit(0ULL, square)) return WHITE_ROOK;
    if (bitboards[WHITE_QUEEN] & setBit(0ULL, square)) return WHITE_QUEEN;
    if (bitboards[WHITE_KING] & setBit(0ULL, square)) return WHITE_KING;

    if (bitboards[BLACK_PAWN] & setBit(0ULL, square)) return BLACK_PAWN;
    if (bitboards[BLACK_KNIGHT] & setBit(0ULL, square)) return BLACK_KNIGHT;
    if (bitboards[BLACK_BISHOP] & setBit(0ULL, square)) return BLACK_BISHOP;
    if (bitboards[BLACK_ROOK] & setBit(0ULL, square)) return BLACK_ROOK;
    if (bitboards[BLACK_QUEEN] & setBit(0ULL, square)) return BLACK_QUEEN;
    if (bitboards[BLACK_KING] & setBit(0ULL, square)) return BLACK_KING;

    return NOPIECE;
}

U64 Position::getOccupiedSquares(Piece piece) {
    return bitboards[piece];
}

U64 Position::getOccupiedSquares(Color color) {
    if (color == WHITE) {
        return  getOccupiedSquares(WHITE_PAWN)   |
                getOccupiedSquares(WHITE_KNIGHT) |
                getOccupiedSquares(WHITE_BISHOP) |
                getOccupiedSquares(WHITE_ROOK)   |
                getOccupiedSquares(WHITE_QUEEN)  |
                getOccupiedSquares(WHITE_KING);
    } else {
        return  getOccupiedSquares(BLACK_PAWN)   |
                getOccupiedSquares(BLACK_KNIGHT) |
                getOccupiedSquares(BLACK_BISHOP) |
                getOccupiedSquares(BLACK_ROOK)   |
                getOccupiedSquares(BLACK_QUEEN)  |
                getOccupiedSquares(BLACK_KING);
    
    }
}

U64 Position::getOccupiedSquares() {
    return getOccupiedSquares(WHITE) | getOccupiedSquares(BLACK);
}

U64 Position::getEmptySquares() {
    return ~getOccupiedSquares();
}


U64 Position::getFriendlyPieces() {
    return currentPlayer == WHITE ? getOccupiedSquares(WHITE) : getOccupiedSquares(BLACK);
}

U64 Position::getFriendlyPieces(PieceType pieceType) {
    return bitboards[pieceType + 6 * currentPlayer];
}

U64 Position::getEnemyPieces() {
    return currentPlayer == WHITE ? getOccupiedSquares(BLACK) : getOccupiedSquares(WHITE);
}

U64 Position::getEnemyPieces(PieceType pieceType) {
    return bitboards[pieceType + 6 * (1 - currentPlayer)];
}

Color Position::getCurrentPlayer() {
    return currentPlayer;
}

bool Position::whiteCanKingsideCastle() {
    return castlingAbility & 1;
}

bool Position::whiteCanQueensideCastle() {
    return castlingAbility & 2;
}

bool Position::blackCanKingsideCastle() {
    return castlingAbility & 4;
}

bool Position::blackCanQueensideCastle() {
    return castlingAbility & 8;
}

U8 Position::getEnPassantTarget() {
    return enPassantTargetSquare;
}

U8 Position::getHalfmoveClock() {
    return halfmoveClock;
}

U8 Position::getFullmoveCounter() {
    return fullmoveCounter;
}

#endif