#ifndef MOVEGENERATORKING_CPP
#define MOVEGENERATORKING_CPP

#include "MoveGenerator.h"

// REGULAR MOVES

void MoveGenerator::precalculateKingMoves() {
    for (int i = 0; i < 64; i++) {
        U64 kingBitboard = setBit(0ULL, i);
        U64 attacks = eastOne(kingBitboard) | westOne(kingBitboard);
        kingBitboard |= attacks;
        attacks |= nortOne(kingBitboard) | soutOne(kingBitboard);
        arrKingMoves[i] = attacks;
    }
}

void MoveGenerator::generateKingMoves(Position& position, int& i) {
    U64 king = position.getFriendlyPieces(KING);
    U64 friendlyPieces = position.getFriendlyPieces();
    U8 from = bitScanForward(king);
    U64 moves = arrKingMoves[from] & ~friendlyPieces;
    while (moves) {
        U8 to = bitScanForward(moves);
        addMove(i, from, to, position.getPieceAt(to) == NOPIECE ? NORMAL : CAPTURE, position.getPieceAt(to), NOPIECE);
        moves &= moves - 1;
    }
}

// CASTLING

void MoveGenerator::generateCastlingMoves(Position& position, int& i) {
    if (position.getCurrentPlayer() == WHITE) {
        if (whiteCanKingsideCastle(position))  addMove(i, E1, G1, KINGSIDE_CASTLE, NOPIECE, NOPIECE);
        if (whiteCanQueensideCastle(position)) addMove(i, E1, C1, QUEENSIDE_CASTLE, NOPIECE, NOPIECE);
    } else {
        if (blackCanKingsideCastle(position)) addMove(i, E8, G8, KINGSIDE_CASTLE, NOPIECE, NOPIECE);
        if (blackCanQueensideCastle(position)) addMove(i, E8, C8, QUEENSIDE_CASTLE, NOPIECE, NOPIECE);
    }
}

bool MoveGenerator::whiteCanKingsideCastle(Position& position) {
    if (!position.whiteCanKingsideCastle()) return false;
    if (isSquareAttackedByColor(position, E1, BLACK)) return false;
    if (isSquareAttackedByColor(position, F1, BLACK)) return false;
    if (isSquareAttackedByColor(position, G1, BLACK)) return false;
    if (position.getPieceAt(F1) != NOPIECE) return false;
    if (position.getPieceAt(G1) != NOPIECE) return false;
    return true;
}

bool MoveGenerator::whiteCanQueensideCastle(Position& position) {
    if (!position.whiteCanQueensideCastle()) return false;
    if (isSquareAttackedByColor(position, C1, BLACK)) return false;
    if (isSquareAttackedByColor(position, D1, BLACK)) return false;
    if (isSquareAttackedByColor(position, E1, BLACK)) return false;
    if (position.getPieceAt(B1) != NOPIECE) return false;
    if (position.getPieceAt(C1) != NOPIECE) return false;
    if (position.getPieceAt(D1) != NOPIECE) return false;
    return true;
}

bool MoveGenerator::blackCanKingsideCastle(Position& position) {
    if (!position.blackCanKingsideCastle()) return false;
    if (isSquareAttackedByColor(position, E8, WHITE)) return false;
    if (isSquareAttackedByColor(position, F8, WHITE)) return false;
    if (isSquareAttackedByColor(position, G8, WHITE)) return false;
    if (position.getPieceAt(F8) != NOPIECE) return false;
    if (position.getPieceAt(G8) != NOPIECE) return false;
    return true;
}

bool MoveGenerator::blackCanQueensideCastle(Position& position) {
    if (!position.blackCanQueensideCastle()) return false;
    if (isSquareAttackedByColor(position, C8, WHITE)) return false;
    if (isSquareAttackedByColor(position, D8, WHITE)) return false;
    if (isSquareAttackedByColor(position, E8, WHITE)) return false;
    if (position.getPieceAt(B8) != NOPIECE) return false;
    if (position.getPieceAt(C8) != NOPIECE) return false;
    if (position.getPieceAt(D8) != NOPIECE) return false;
    return true;
}

#endif