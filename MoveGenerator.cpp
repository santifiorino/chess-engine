#include "MoveGenerator.h"
#include "Position.h"
#include "Utils.h"

MoveGenerator::MoveGenerator() {
    
}

int MoveGenerator::generateMoves(Position &position) {
    int i = 0;
    // Pawn pushes
    generatePawnPushes(position, i);
    // Pawn captures
    generatePawnCaptures(position, i);
    return i;
}

uint64 MoveGenerator::whitePawnsAbleToPush(uint64 whitePawns, uint64 empty) {
    return soutOne(empty) & whitePawns;
}

uint64 MoveGenerator::blackPawnsAbleToPush(uint64 blackPawns, uint64 empty) {
    return nortOne(empty) & blackPawns;
}

uint64 MoveGenerator::whitePawnsAbleToDoublePush(uint64 whitePawns, uint64 empty) {
    uint64 emptyRank3 = soutOne(empty & RANK_4) & empty;
    return whitePawnsAbleToPush(whitePawns, emptyRank3);
}

uint64 MoveGenerator::blackPawnsAbleToDoublePush(uint64 blackPawns, uint64 empty) {
    uint64 emptyRank6 = nortOne(empty & RANK_5) & empty;
    return blackPawnsAbleToPush(blackPawns, emptyRank6);
}

void MoveGenerator::generatePawnPush(uint64 pushes, int& i, Color color, bool isDoublePush) {
    while (pushes) {
        uint8 from = bitScanForward(pushes);
        if (from == 0) break;
        uint8 to = from + (isDoublePush ? 16 : 8) * (color == WHITE ? 1 : -1);
        Move move = {from, to, NORMAL, EMPTY, EMPTY};
        legalMoves[i] = move;
        i++;
        pushes &= pushes - 1;
    }
}

void MoveGenerator::generatePawnPushes(Position& position, int& i){
    if (position.getCurrentPlayer() == WHITE) {
        uint64 whitePawns = position.getBitboard(WHITE_PAWN);
        uint64 empty = position.getEmptySquares();
        generatePawnPush(whitePawnsAbleToPush(whitePawns, empty), i, WHITE, false);
        generatePawnPush(whitePawnsAbleToDoublePush(whitePawns, empty), i, WHITE, true);
    } else {
        uint64 blackPawns = position.getBitboard(BLACK_PAWN);
        uint64 empty = position.getEmptySquares();
        generatePawnPush(blackPawnsAbleToPush(blackPawns, empty), i, BLACK, false);
        generatePawnPush(blackPawnsAbleToDoublePush(blackPawns, empty), i, BLACK, true);
    }
}

uint64 MoveGenerator::whitePawnEastAttacks(uint64 whitePawns) { return noEaOne(whitePawns); }
uint64 MoveGenerator::whitePawnWestAttacks(uint64 whitePawns) { return noWeOne(whitePawns); }

uint64 MoveGenerator::blackPawnEastAttacks(uint64 blackPawns) { return soEaOne(blackPawns); }
uint64 MoveGenerator::blackPawnWestAttacks(uint64 blackPawns) { return soWeOne(blackPawns); }

uint64 MoveGenerator::whitePawnsAbleToCaptureEast(uint64 whitePawns, uint64 blackPieces) {
    return whitePawns & blackPawnWestAttacks(blackPieces);
}

uint64 MoveGenerator::whitePawnsAbleToCaptureWest(uint64 whitePawns, uint64 blackPieces) {
    return whitePawns & blackPawnEastAttacks(blackPieces);
}

uint64 MoveGenerator::blackPawnsAbleToCaptureEast(uint64 blackPawns, uint64 whitePieces) {
    return blackPawns & whitePawnWestAttacks(whitePieces);
}

uint64 MoveGenerator::blackPawnsAbleToCaptureWest(uint64 blackPawns, uint64 whitePieces) {
    return blackPawns & whitePawnEastAttacks(whitePieces);
}

void MoveGenerator::generateDirectedPawnCaptures(Position& position, int& i, Direction direction) {
    Color color = position.getCurrentPlayer();
    uint64 pawns, enemyPieces, ableToCapture;
    uint8 offset;
    if (color == WHITE){
        pawns = position.getBitboard(WHITE_PAWN);
        enemyPieces = position.getBlackOccupiedSquares();
        ableToCapture = direction == EAST ? whitePawnsAbleToCaptureEast(pawns, enemyPieces) : whitePawnsAbleToCaptureWest(pawns, enemyPieces);
        offset = direction == EAST ? NORTH_EAST : NORTH_WEST;
    } else {
        pawns = position.getBitboard(BLACK_PAWN);
        enemyPieces = position.getWhiteOccupiedSquares();
        ableToCapture = direction == EAST ? blackPawnsAbleToCaptureEast(pawns, enemyPieces) : blackPawnsAbleToCaptureWest(pawns, enemyPieces);
        offset = direction == EAST ? SOUTH_EAST : SOUTH_WEST;
    }
    while (ableToCapture) {
        uint8 from = bitScanForward(ableToCapture);
        uint8 to = from + offset;
        Move move = {from, to, CAPTURE, EMPTY, position.getPieceAt(to)};
        legalMoves[i] = move;
        i++;
        ableToCapture &= ableToCapture - 1;
    }
}

void MoveGenerator::generatePawnCaptures(Position& position, int& i) {
    if (position.getCurrentPlayer() == WHITE) {
        generateDirectedPawnCaptures(position, i, EAST);
        generateDirectedPawnCaptures(position, i, WEST);
    } else {
        generateDirectedPawnCaptures(position, i, EAST);
        generateDirectedPawnCaptures(position, i, WEST);
    }
}
    