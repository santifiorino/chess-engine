#include "MoveGenerator.h"
#include "Position.h"
#include "Utils.h"

MoveGenerator::MoveGenerator() {
    precalculatePawnAttacks();
    precalculateKnightMoves();
}

int MoveGenerator::generateMoves(Position &position) {
    int i = 0;
    // Pawn Moves | TODO: Add promotion
    generatePawnPushes(position, i);
    generatePawnCaptures(position, i);
    // Knight Moves
    generateKnightMoves(position, i);
    return i;
}

// PAWN MOVES
// Pawn pushes
uint64 MoveGenerator::pawnsAbleToPush(uint64 pawns, uint64 empty, Color color) {
    return color == WHITE ? soutOne(empty) & pawns : nortOne(empty) & pawns;
}

uint64 MoveGenerator::pawnsAbleToDoublePush(uint64 pawns, uint64 empty, Color color) {
    uint64 doublePushRank = (color == WHITE ? RANK_4 : RANK_5) & empty;
    uint64 emptyRank = color == WHITE ? soutOne(doublePushRank) : nortOne(doublePushRank);
    emptyRank &= empty;
    return pawnsAbleToPush(pawns, emptyRank, color);
}

void MoveGenerator::generatePawnPush(uint64 pawns, int& i, Color color, bool isDoublePush) {
    while (pawns) {
        uint8 from = bitScanForward(pawns);
        if (from == 0) break;
        uint8 to = from + (isDoublePush ? 16 : 8) * (color == WHITE ? 1 : -1);
        Move move = {from, to, NORMAL, EMPTY, EMPTY};
        legalMoves[i] = move;
        i++;
        pawns &= pawns - 1;
    }
}

void MoveGenerator::generatePawnPushes(Position& position, int& i){
    if (position.getCurrentPlayer() == WHITE) {
        uint64 whitePawns = position.getBitboard(WHITE_PAWN);
        uint64 empty = position.getEmptySquares();
        generatePawnPush(pawnsAbleToPush(whitePawns, empty, WHITE), i, WHITE, false);
        generatePawnPush(pawnsAbleToDoublePush(whitePawns, empty, WHITE), i, WHITE, true);
    } else {
        uint64 blackPawns = position.getBitboard(BLACK_PAWN);
        uint64 empty = position.getEmptySquares();
        generatePawnPush(pawnsAbleToPush(blackPawns, empty, BLACK), i, BLACK, false);
        generatePawnPush(pawnsAbleToDoublePush(blackPawns, empty, BLACK), i, BLACK, true);
    }
}

// Pawn attacks
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
                move = {from, to, EN_PASSANT, color == WHITE ? BLACK_PAWN : WHITE_PAWN, EMPTY};
            } else {
                move = {from, to, CAPTURE, position.getPieceAt(to), EMPTY};
            }
            legalMoves[i] = move;
            i++;
            attacks &= attacks - 1;
        }
        pawns &= pawns - 1;
    }
}

// KNIGHT MOVES
void MoveGenerator::precalculateKnightMoves() {
    for (int i = 0; i < 64; i++) {
        uint64 knight = 1ULL << i;
        uint64 l1 = (knight >> 1) & 0x7F7F7F7F7F7F7F7FULL;
        uint64 l2 = (knight >> 2) & 0x3F3F3F3F3F3F3F3FULL;
        uint64 r1 = (knight << 1) & 0xFEFEFEFEFEFEFEFEULL;
        uint64 r2 = (knight << 2) & 0xFCFCFCFCFCFCFCFCULL;
        uint64 h1 = l1 | r1;
        uint64 h2 = l2 | r2;
        arrKnightMoves[i] = (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
    }
}

void MoveGenerator::generateKnightMoves(Position& position, int& i) {
    Color color = position.getCurrentPlayer();
    uint64 knights = color == WHITE ? position.getBitboard(WHITE_KNIGHT) : position.getBitboard(BLACK_KNIGHT);
    uint64 friendlyPieces = color == WHITE ? position.getWhiteOccupiedSquares() : position.getBlackOccupiedSquares();
    uint64 enemyPieces = color == WHITE ? position.getBlackOccupiedSquares() : position.getWhiteOccupiedSquares();
    while (knights) {
        uint8 from = bitScanForward(knights);
        uint64 moves = arrKnightMoves[from] & ~friendlyPieces;
        while (moves) {
            uint8 to = bitScanForward(moves);
            Move move = {from, to, position.getPieceAt(to) == EMPTY ? NORMAL : CAPTURE, position.getPieceAt(to), EMPTY};
            legalMoves[i] = move;
            i++;
            moves &= moves - 1;
        }
        knights &= knights - 1;
    }
}