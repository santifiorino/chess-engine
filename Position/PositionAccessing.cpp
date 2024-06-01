#ifndef POSITIONACCESSING_CPP
#define POSITIONACCESSING_CPP

#include "Position.h"

Piece Position::getPieceAt(int square) {
    U64 squareBit = setBit(0ULL, square);
    if (bitboards[WHITE_PAWN] & squareBit) return WHITE_PAWN;
    if (bitboards[WHITE_KNIGHT] & squareBit) return WHITE_KNIGHT;
    if (bitboards[WHITE_BISHOP] & squareBit) return WHITE_BISHOP;
    if (bitboards[WHITE_ROOK] & squareBit) return WHITE_ROOK;
    if (bitboards[WHITE_QUEEN] & squareBit) return WHITE_QUEEN;
    if (bitboards[WHITE_KING] & squareBit) return WHITE_KING;

    if (bitboards[BLACK_PAWN] & squareBit) return BLACK_PAWN;
    if (bitboards[BLACK_KNIGHT] & squareBit) return BLACK_KNIGHT;
    if (bitboards[BLACK_BISHOP] & squareBit) return BLACK_BISHOP;
    if (bitboards[BLACK_ROOK] & squareBit) return BLACK_ROOK;
    if (bitboards[BLACK_QUEEN] & squareBit) return BLACK_QUEEN;
    if (bitboards[BLACK_KING] & squareBit) return BLACK_KING;

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
    return castlingAbility & 0b0001;
}

bool Position::whiteCanQueensideCastle() {
    return castlingAbility & 0b0010;
}

bool Position::blackCanKingsideCastle() {
    return castlingAbility & 0b0100;
}

bool Position::blackCanQueensideCastle() {
    return castlingAbility & 0b1000;
}

U8 Position::getCastlingAbility() {
    return castlingAbility;
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

bool Position::halfmoveClockIsFifty() {
    return halfmoveClock >= 100;
}

U64 Position::getPositionHash() {
    return positionHash;
}

bool Position::isInsufficientMaterial() {
    U64 whitePieces = getOccupiedSquares(WHITE);
    U64 blackPieces = getOccupiedSquares(BLACK);
    U64 whiteAux = 0ULL;
    U64 blackAux = 0ULL;

    // King vs king
    whiteAux |= bitboards[WHITE_KING];
    blackAux |= bitboards[BLACK_KING];
    if (whiteAux == whitePieces &&
        blackAux == blackPieces) return true;

    // King vs king and bishop
    whiteAux |= bitboards[WHITE_BISHOP];
    if (whiteAux == whitePieces &&
        countOnes(bitboards[WHITE_BISHOP]) == 1 &&
        blackAux == blackPieces) return true;
    whiteAux ^= bitboards[WHITE_BISHOP];

    blackAux |= bitboards[BLACK_BISHOP];
    if (whiteAux == whitePieces &&
        blackAux == blackPieces &&
        countOnes(bitboards[BLACK_BISHOP]) == 1) return true;
    blackAux ^= bitboards[BLACK_BISHOP];

    // King vs king and knight
    whiteAux |= bitboards[WHITE_KNIGHT];
    if (whiteAux == whitePieces &&
        countOnes(bitboards[WHITE_KNIGHT]) == 1 &&
        blackAux == blackPieces) return true;
    whiteAux ^= bitboards[WHITE_KNIGHT];

    blackAux |= bitboards[BLACK_KNIGHT];
    if (whiteAux == whitePieces &&
        blackAux == blackPieces &&
        countOnes(bitboards[BLACK_KNIGHT]) == 1) return true;
    blackAux ^= bitboards[BLACK_KNIGHT];

    return false;
}

int Position::getMaterialScore() {
    return materialScore;
}

#endif