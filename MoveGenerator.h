#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "Constants.h"
#include "Position.h"

enum MoveType {
    NORMAL,
    CAPTURE,
    EN_PASSANT,
    CASTLE_KINGSIDE,
    CASTLE_QUEENSIDE,
    PROMOTION
};

struct Move {
    uint8 from;
    uint8 to;
    MoveType type;
    Piece promotion;
    Piece captured;
};

class MoveGenerator {
    public:
        Move legalMoves[218];
        int generateMoves(Position &position);
    private:
        uint64 whitePawnsAbleToPush(uint64 whitePawns, uint64 empty);
};

#endif