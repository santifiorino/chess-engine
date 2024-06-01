#ifndef MOVEGENERATORSLIDERS_CPP
#define MOVEGENERATORSLIDERS_CPP

#include "MoveGenerator.h"

void MoveGenerator::precalculateSliderMoves(PieceType pieceType) {
    for (int i = 0; i < 64; i++) {
        U64 attackMask = (pieceType == BISHOP) ? arrBishopOccupancyMask[i] : arrRookOccupancyMask[i];
        int relevantBitsCount = (pieceType == BISHOP) ? arrBishopRelevantBits[i] : arrRookRelevantBits[i];
        for (int index = 0; index < 1 << relevantBitsCount; index++) {
            U64 occupancy = placeNumberInMask(index, attackMask);
            if (pieceType == BISHOP) {
                int magicIndex = (int) ((occupancy * arrBishopMagicNumbers[i]) >> (64 - relevantBitsCount));
                arrBishopMoves[i][magicIndex] = bishopMovesMapOnTheFly(i, occupancy);
            } else {
                int magicIndex = (int) ((occupancy * arrRookMagicNumbers[i]) >> (64 - relevantBitsCount));
                arrRookMoves[i][magicIndex] = rookMovesMapOnTheFly(i, occupancy);
            }
        }
    }
}

void MoveGenerator::generateSliderMoves(Position& position, int& i, PieceType pieceType) {
    U64 friendlyPieces = position.getFriendlyPieces();
    U64 pieces = position.getFriendlyPieces(pieceType);
    U64 occupancy = position.getOccupiedSquares();
    while (pieces) {
        U8 from = bitScanForward(pieces);
        U64 moves;
        if (pieceType == BISHOP) moves = getBishopMoves(from, occupancy);
        if (pieceType == ROOK) moves = getRookMoves(from, occupancy);
        if (pieceType == QUEEN) moves = getQueenMoves(from, occupancy);
        moves &= ~friendlyPieces;
        while (moves) {
            U8 to = bitScanForward(moves);
            addMove(i, from, to, position.getPieceAt(to) == NOPIECE ? NORMAL : CAPTURE, position.getPieceAt(to), NOPIECE);
            moves &= moves - 1;
        }
        pieces &= pieces - 1;
    }
}

U64 MoveGenerator::getBishopMoves(U8 square, U64 occupancy) {
    occupancy &= arrBishopOccupancyMask[square];
    occupancy *= arrBishopMagicNumbers[square];
    occupancy >>= 64 - arrBishopRelevantBits[square];
    return arrBishopMoves[square][(int) occupancy];
}

U64 MoveGenerator::getRookMoves(U8 square, U64 occupancy) {
    occupancy &= arrRookOccupancyMask[square];
    occupancy *= arrRookMagicNumbers[square];
    occupancy >>= 64 - arrRookRelevantBits[square];
    return arrRookMoves[square][(int) occupancy];
}

U64 MoveGenerator::getQueenMoves(U8 square, U64 occupancy) {
    return getBishopMoves(square, occupancy) | getRookMoves(square, occupancy);
}

U64 MoveGenerator::bishopMovesMapOnTheFly(int square, U64 occupancy) {
    U64 attack = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;
    // North East - South East - South West - North West
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }

    return attack;
}

U64 MoveGenerator::rookMovesMapOnTheFly(int square, U64 occupancy) {
    U64 attack = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    // North - South - East - West
    for (r = tr + 1; r <= 7; r++) {
        attack = setBit(attack, toBoardIndex(r, tf));
        if (getBit(occupancy, toBoardIndex(r, tf))) break;
    }
    for (r = tr - 1; r >= 0; r--) {
        attack = setBit(attack, toBoardIndex(r, tf));
        if (getBit(occupancy, toBoardIndex(r, tf))) break;
    }
    for (f = tf + 1; f <= 7; f++) {
        attack = setBit(attack, toBoardIndex(tr, f));
        if (getBit(occupancy, toBoardIndex(tr, f))) break;
    }
    for (f = tf - 1; f >= 0; f--) {
        attack = setBit(attack, toBoardIndex(tr, f));
        if (getBit(occupancy, toBoardIndex(tr, f))) break;
    }

    return attack;
}

#endif