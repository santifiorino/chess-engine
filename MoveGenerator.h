#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "Utils.h"
#include "Position.h"

enum MoveType {
    NORMAL,
    CAPTURE,
    EN_PASSANT,
    CASTLE_KINGSIDE,
    CASTLE_QUEENSIDE,
    PROMOTION,
    PROMOTION_CAPTURE
};

struct Move {
    U8 from;
    U8 to;
    MoveType type;
    Piece captured;
    Piece promotion;
};

class MoveGenerator {
    public:
        MoveGenerator();
        Move legalMoves[218];
        int generateMoves(Position &position);
    private:
        void addMove(int& i, U8 from, U8 to, MoveType type, Piece captured, Piece promotion);
        // PAWN MOVES
        // Pawn pushes
        U64 getPawnsAbleToPush(U64 pawns, U64 empty, Color color);
        U64 getPawnsAbleToDoublePush(U64 pawns, U64 empty, Color color);
        void generatePawnPush(U64 pushes, int& i, Color color, bool isDoublePush);
        void generatePawnPushes(Position& position, int& i);
        // Pawn attacks
        U64 arrPawnAttacks[2][64];
        void precalculatePawnAttacks();
        void generatePawnCaptures(Position& position, int& i);

        // KNIGHT MOVES
        U64 arrKnightMoves[64];
        void precalculateKnightMoves();
        void generateKnightMoves(Position &position, int &i);

        // KING MOVES
        U64 arrKingMoves[64];
        void precalculateKingMoves();
        void generateKingMoves(Position &position, int &i);

        // SLIDING PIECES
        // Mask relevant occupancy bits to form a key for magic bitboards
        U64 bishopRelevantOccupancyMask[64];
        U64 rookRelevantOccupancyMask[64];
        void generateMasks();
        U64 maskBishop(int square);
        U64 maskRook(int square);
        // Bishop and Rook attacks on the fly (slow, only for magic number generation)
        U64 getBishopAttackOnTheFly(int square, U64 occupancy);
};

#endif