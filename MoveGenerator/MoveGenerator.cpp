#ifndef MOVEGENERATOR_CPP
#define MOVEGENERATOR_CPP

#include "MoveGenerator.h"

MoveGenerator::MoveGenerator() {
    precalculatePawnAttacks();
    precalculateKnightMoves();
    precalculateKingMoves();
    // Sliding Pieces
    generateRelevantOccupancyMasks();
    // precalculateMagicNumbers(); // Run it once to generate the magic numbers
    precalculateSliderMoves(BISHOP);
    precalculateSliderMoves(ROOK);
}

int MoveGenerator::generateMoves(Position &position) {
    int i = 0;
    generatePawnPushes(position, i);
    generatePawnCaptures(position, i);
    generateKnightMoves(position, i);
    generateKingMoves(position, i);
    generateSliderMoves(position, i, BISHOP);
    generateSliderMoves(position, i, ROOK);
    generateSliderMoves(position, i, QUEEN);
    generateCastlingMoves(position, i);
    return i;
}

void MoveGenerator::addMove(int& i, U8 from, U8 to, MoveType type, Piece captured, PieceType promotion) {
    Move move = {from, to, type, captured, promotion};
    legalMoves[i] = move;
    i++;
}

bool MoveGenerator::isSquareAttackedByColor(Position& position, U8 square, Color color) {
    U64 occupancy = position.getOccupiedSquares();
    if (color == WHITE) {
        // If a black pawn is there, and it can attack a white pawn, then it's attacked by that pawn
        // If a black knight is there, and it can attack a white knight, then it's attacked by that knight
        // etc.
        if (arrPawnAttacks[BLACK][square] & position.getOccupiedSquares(WHITE_PAWN)) return true;
        if (arrKnightMoves[square] & position.getOccupiedSquares(WHITE_KNIGHT)) return true;
        if (getBishopMoves(square, occupancy) & position.getOccupiedSquares(WHITE_BISHOP)) return true;
        if (getRookMoves(square, occupancy) & position.getOccupiedSquares(WHITE_ROOK)) return true;
        if (getQueenMoves(square, occupancy) & position.getOccupiedSquares(WHITE_QUEEN)) return true;
        if (arrKingMoves[square] & position.getOccupiedSquares(WHITE_KING)) return true;
        return false;
    } else {
        if (arrPawnAttacks[WHITE][square] & position.getOccupiedSquares(BLACK_PAWN)) return true;
        if (arrKnightMoves[square] & position.getOccupiedSquares(BLACK_KNIGHT)) return true;
        if (getBishopMoves(square, occupancy) & position.getOccupiedSquares(BLACK_BISHOP)) return true;
        if (getRookMoves(square, occupancy) & position.getOccupiedSquares(BLACK_ROOK)) return true;
        if (getQueenMoves(square, occupancy) & position.getOccupiedSquares(BLACK_QUEEN)) return true;
        if (arrKingMoves[square] & position.getOccupiedSquares(BLACK_KING)) return true;
        return false;
    }
}

#endif