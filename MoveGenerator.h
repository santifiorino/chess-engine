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
        uint64 whitePawnsAbleToDoublePush(uint64 whitePawns, uint64 empty);

        uint64 blackPawnsAbleToPush(uint64 blackPawns, uint64 empty);
        uint64 blackPawnsAbleToDoublePush(uint64 blackPawns, uint64 empty);

        void generatePawnPushes(uint64 pushes, int& i, Color color, bool isDoublePush);
};

#endif