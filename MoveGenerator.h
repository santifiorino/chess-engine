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
    uint8 from;
    uint8 to;
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
        void addMove(int& i, uint8 from, uint8 to, MoveType type, Piece captured, Piece promotion);
        // PAWN MOVES
        // Pawn pushes
        uint64 pawnsAbleToPush(uint64 pawns, uint64 empty, Color color);
        uint64 pawnsAbleToDoublePush(uint64 pawns, uint64 empty, Color color);
        void generatePawnPush(uint64 pushes, int& i, Color color, bool isDoublePush);
        void generatePawnPushes(Position& position, int& i);
        // Pawn attacks
        uint64 arrPawnAttacks[2][64];
        void precalculatePawnAttacks();
        void generatePawnCaptures(Position& position, int& i);

        // KNIGHT MOVES
        uint64 arrKnightMoves[64];
        void precalculateKnightMoves();
        void generateKnightMoves(Position &position, int &i);
};

#endif