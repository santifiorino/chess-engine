#include "MoveGenerator.h"
#include "Position.h"
#include "Utils.h"

MoveGenerator::MoveGenerator() {
    precalculatePawnAttacks();
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

void MoveGenerator::precalculatePawnAttacks() {
    for (int i = 0; i < 64; i++) {
        uint64 pawn = 1ULL << i;
        arrPawnAttacks[WHITE][i] = noEaOne(pawn) | noWeOne(pawn);
        arrPawnAttacks[BLACK][i] = soEaOne(pawn) | soWeOne(pawn);
    }
}

void MoveGenerator::generatePawnCaptures(Position& position, int& i) {
    Color color = position.getCurrentPlayer();
    uint64 pawns = color == WHITE ? position.getBitboard(WHITE_PAWN) : position.getBitboard(BLACK_PAWN);
    uint64 enemyPieces = color == WHITE ? position.getBlackOccupiedSquares() : position.getWhiteOccupiedSquares();
    enemyPieces |= 1ULL << position.getEnPassantTarget();
    while (pawns) {
        uint8 from = bitScanForward(pawns);
        uint64 attacks = arrPawnAttacks[color][from] & enemyPieces;
        while (attacks) {
            uint8 to = bitScanForward(attacks);
            Move move;
            if (to == position.getEnPassantTarget()) {
                move = {from, to, EN_PASSANT, EMPTY, color == WHITE ? BLACK_PAWN : WHITE_PAWN};
            } else {
                move = {from, to, CAPTURE, EMPTY, position.getPieceAt(to)};
            }
            legalMoves[i] = move;
            i++;
            attacks &= attacks - 1;
        }
        pawns &= pawns - 1;
    }
}
    