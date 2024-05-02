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

void MoveGenerator::generateLegalMoves(Position &position) {
    for (int i = 0; i < pseudoLegalMovesCount; i++) legalMove[i] = false;
    generatePseudoLegalMoves(position);
    legalMovesCount = 0;
    for (int i = 0; i < pseudoLegalMovesCount; i++) {
        Move move = pseudoLegalMoves[i];
        if (checkMoveLegality(position, move)) {
            legalMove[legalMovesCount] = true;
            legalMovesCount++;
        }
    }
}

void MoveGenerator::generatePseudoLegalMoves(Position &position) {
    int i = 0;
    generatePawnPushes(position, i);
    generatePawnCaptures(position, i);
    generateKnightMoves(position, i);
    generateKingMoves(position, i);
    generateSliderMoves(position, i, BISHOP);
    generateSliderMoves(position, i, ROOK);
    generateSliderMoves(position, i, QUEEN);
    generateCastlingMoves(position, i);
    pseudoLegalMovesCount = i;
}

bool MoveGenerator::checkMoveLegality(Position &position, Move move) {
    // Save position data and make the move
    Piece king = position.getCurrentPlayer() == WHITE ? WHITE_KING : BLACK_KING;
    U8 castlingAbility = position.getCastlingAbility();
    U8 enPassantSquares = position.getEnPassantTarget();
    U8 halfmoveClock = position.getHalfmoveClock();
    U8 fullmoveCounter = position.getFullmoveCounter();
    position.makeMove(move);
    // If the king is attacked, the move is illegal
    U8 kingSquare = bitScanForward(position.getOccupiedSquares(king));
    bool res = !(isSquareAttackedByColor(position, kingSquare, position.getCurrentPlayer()));
    // Unmake the move restoring the position data
    position.unmakeMove(move, castlingAbility, enPassantSquares, halfmoveClock, fullmoveCounter);
    return res;
}

void MoveGenerator::addMove(int& i, U8 from, U8 to, MoveType type, Piece captured, PieceType promotion) {
    Move move = {from, to, type, captured, promotion};
    pseudoLegalMoves[i] = move;
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

int MoveGenerator::getMoveIndex(Position &position, U8 from, U8 to) {
    for (int i = 0; i < pseudoLegalMovesCount; i++) {
        if (pseudoLegalMoves[i].from == from && pseudoLegalMoves[i].to == to) {
            return legalMove[i] ? i : -1;
        }
    }
    return -1;
}

U64 MoveGenerator::getMovesFrom(Position &position, U8 square) {
    U64 to = 0ULL;
    for (int i = 0; i < pseudoLegalMovesCount; i++) {
        if (pseudoLegalMoves[i].from == square) {
            if (legalMove[i])
                to |= setBit(0ULL, pseudoLegalMoves[i].to);
        }
    }
    return to;
}

#endif