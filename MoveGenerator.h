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
        MoveGenerator();
        Move legalMoves[218];
        int generateMoves(Position &position);
        uint64 arrPawnAttacks[2][64];
    private:
        // PAWN MOVES
        // Pawn push generation
        uint64 whitePawnsAbleToPush(uint64 whitePawns, uint64 empty);
        uint64 whitePawnsAbleToDoublePush(uint64 whitePawns, uint64 empty);

        uint64 blackPawnsAbleToPush(uint64 blackPawns, uint64 empty);
        uint64 blackPawnsAbleToDoublePush(uint64 blackPawns, uint64 empty);

        void generatePawnPush(uint64 pushes, int& i, Color color, bool isDoublePush);
        void generatePawnPushes(Position& position, int& i);
        // Pawn attack generation
        uint64 whitePawnEastAttacks(uint64 whitePawns);
        uint64 whitePawnWestAttacks(uint64 whitePawns);

        uint64 blackPawnEastAttacks(uint64 blackPawns);
        uint64 blackPawnWestAttacks(uint64 blackPawns);

        uint64 whitePawnsAbleToCaptureEast(uint64 whitePawns, uint64 blackPieces);
        uint64 whitePawnsAbleToCaptureWest(uint64 whitePawns, uint64 blackPieces);

        uint64 blackPawnsAbleToCaptureEast(uint64 blackPawns, uint64 whitePieces);
        uint64 blackPawnsAbleToCaptureWest(uint64 blackPawns, uint64 whitePieces);

        void generateDirectedPawnCaptures(Position& position, int& i, Direction direction);

        void generatePawnCaptures(Position& position, int& i);
};

#endif