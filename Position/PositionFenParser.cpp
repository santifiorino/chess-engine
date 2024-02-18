#ifndef POSITIONFENPARSER_CPP
#define POSITIONFENPARSER_CPP

#include "Position.h"

void Position::parsePiecePlacement(const char* FEN, int& i){
    int file = 7;
    int rank = 0;
    while (FEN[i] != ' ') {
        if (FEN[i] == '/') {
            rank = 0;
            file -= 1;
            i++;
            continue;
        }
        if (isDigit(FEN[i])){
            rank += toDigit(FEN[i]);
            i++;
            continue;
        }
        Piece piece = charToPiece(FEN[i]);
        bitboards[piece] |= (1ULL << toBoardIndex(file, rank));
        rank++;
        i++;
    }
    i++;
}

Piece Position::charToPiece(char fenChar) {
    switch (fenChar) {
        case 'P': return WHITE_PAWN;
        case 'N': return WHITE_KNIGHT;
        case 'B': return WHITE_BISHOP;
        case 'R': return WHITE_ROOK;
        case 'Q': return WHITE_QUEEN;
        case 'K': return WHITE_KING;
        case 'p': return BLACK_PAWN;
        case 'n': return BLACK_KNIGHT;
        case 'b': return BLACK_BISHOP;
        case 'r': return BLACK_ROOK;
        case 'q': return BLACK_QUEEN;
        case 'k': return BLACK_KING;
    }
    return NOPIECE;
}

void Position::parsePlayer(const char* FEN, int& i){
    currentPlayer = (FEN[i] == 'w') ? WHITE : BLACK;
    i += 2;
}

void Position::parseCastlingAbility(const char* FEN, int& i){
    castlingAbility = 0;
    if (FEN[i] != '-') {
        while (FEN[i] != ' ') {
            switch (FEN[i]) {
                case 'K': castlingAbility |= 1; break;
                case 'Q': castlingAbility |= 2; break;
                case 'k': castlingAbility |= 4; break;
                case 'q': castlingAbility |= 8; break;
            }
            i++;
        }
    } else {
        i++;
    }
    i++;
}

void Position::parseEnPassantTarget(const char* FEN, int& i){
    enPassantTargetSquare = 64;
    if (FEN[i] == '-') {
        i += 2;
    } else {
        enPassantTargetSquare = toBoardIndex(FEN[i + 1] - '1', FEN[i] - 'a');
        i += 3;
    }
}

void Position::parseHalfmoveClock(const char* FEN, int& i){
    halfmoveClock = 0;
    while (FEN[i] != ' ') {
        halfmoveClock = halfmoveClock * 10 + toDigit(FEN[i]);
        i++;
    }
    i++;
}

void Position::parseFullmoveCounter(const char* FEN, int& i){
    fullmoveCounter = 0;
    while (FEN[i] != '\0') {
        fullmoveCounter = fullmoveCounter * 10 + toDigit(FEN[i]);
        i++;
    }
}

#endif