#include "MoveGenerator.h"
#include "Position.h"
#include "Utils.h"

MoveGenerator::MoveGenerator() {
    precalculatePawnAttacks();
    precalculateKnightMoves();
    precalculateKingMoves();
}

int MoveGenerator::generateMoves(Position &position) {
    int i = 0;
    // Pawn Moves | TODO: Add promotion
    generatePawnPushes(position, i);
    generatePawnCaptures(position, i);
    // Knight Moves
    generateKnightMoves(position, i);
    // King Moves
    generateKingMoves(position, i);
    return i;
}

void MoveGenerator::addMove(int& i, uint8 from, uint8 to, MoveType type, Piece captured, Piece promotion) {
    Move move = {from, to, type, captured, promotion};
    legalMoves[i] = move;
    i++;
}

// PAWN MOVES
// Pawn pushes
// https://www.chessprogramming.org/Pawn_Pushes_(Bitboards)
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
        if (color == WHITE && to >= 56 || color == BLACK && to < 8) {
            addMove(i, from, to, PROMOTION, EMPTY, color == WHITE ? WHITE_QUEEN : BLACK_QUEEN);
            addMove(i, from, to, PROMOTION, EMPTY, color == WHITE ? WHITE_ROOK : BLACK_ROOK);
            addMove(i, from, to, PROMOTION, EMPTY, color == WHITE ? WHITE_BISHOP : BLACK_BISHOP);
            addMove(i, from, to, PROMOTION, EMPTY, color == WHITE ? WHITE_KNIGHT : BLACK_KNIGHT);
        } else {
            addMove(i, from, to, NORMAL, EMPTY, EMPTY);
        }
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
    if (position.getEnPassantTarget() != 64) enemyPieces |= 1ULL << position.getEnPassantTarget();
    while (pawns) {
        uint8 from = bitScanForward(pawns);
        uint64 attacks = arrPawnAttacks[color][from] & enemyPieces;
        while (attacks) {
            uint8 to = bitScanForward(attacks);
            Move move;
            if (to == position.getEnPassantTarget()) {
                addMove(i, from, to, EN_PASSANT, color == WHITE ? BLACK_PAWN : WHITE_PAWN, EMPTY);
            } else {
                if (color == WHITE && to >= 56 || color == BLACK && to < 8) {
                    addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), color == WHITE ? WHITE_QUEEN : BLACK_QUEEN);
                    addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), color == WHITE ? WHITE_ROOK : BLACK_ROOK);
                    addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), color == WHITE ? WHITE_BISHOP : BLACK_BISHOP);
                    addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), color == WHITE ? WHITE_KNIGHT : BLACK_KNIGHT);
                } else {
                    addMove(i, from, to, CAPTURE, position.getPieceAt(to), EMPTY);
                }
            }
            attacks &= attacks - 1;
        }
        pawns &= pawns - 1;
    }
}

// KNIGHT MOVES
// https://www.chessprogramming.org/Knight_Pattern
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

// KING MOVES
// https://www.chessprogramming.org/King_Pattern
void MoveGenerator::precalculateKingMoves() {
    uint64 sqBB = 1;
    for (int sq = 0; sq < 64; sq++, sqBB <<= 1) {
        uint64 kingSet = sqBB;
        uint64 attacks = eastOne(kingSet) | westOne(kingSet);
        kingSet |= attacks;
        attacks |= nortOne(kingSet) | soutOne(kingSet);
        arrKingMoves[sq] = attacks;
    }
}

void MoveGenerator::generateKingMoves(Position& position, int& i) {
    Color color = position.getCurrentPlayer();
    uint64 king = color == WHITE ? position.getBitboard(WHITE_KING) : position.getBitboard(BLACK_KING);
    uint64 friendlyPieces = color == WHITE ? position.getWhiteOccupiedSquares() : position.getBlackOccupiedSquares();
    uint8 from = bitScanForward(king);
    uint64 moves = arrKingMoves[from] & ~friendlyPieces;
    while (moves) {
        uint8 to = bitScanForward(moves);
        Move move = {from, to, position.getPieceAt(to) == EMPTY ? NORMAL : CAPTURE, position.getPieceAt(to), EMPTY};
        legalMoves[i] = move;
        i++;
        moves &= moves - 1;
    }
}