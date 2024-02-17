#include "MoveGenerator.h"
#include "Position.h"
#include "Utils.h"

MoveGenerator::MoveGenerator() {
    precalculatePawnAttacks();
    precalculateKnightMoves();
    precalculateKingMoves();
    // SLIDING PIECES
    generateMasks();
}

int MoveGenerator::generateMoves(Position &position) {
    int i = 0;
    // Pawn Moves
    generatePawnPushes(position, i);
    generatePawnCaptures(position, i);
    // Knight Moves
    generateKnightMoves(position, i);
    // King Moves
    generateKingMoves(position, i);
    return i;
}

void MoveGenerator::addMove(int& i, U8 from, U8 to, MoveType type, Piece captured, Piece promotion) {
    Move move = {from, to, type, captured, promotion};
    legalMoves[i] = move;
    i++;
}

// PAWN MOVES
// Pawn pushes
// https://www.chessprogramming.org/Pawn_Pushes_(Bitboards)
U64 MoveGenerator::getPawnsAbleToPush(U64 pawns, U64 empty, Color color) {
    return color == WHITE ? soutOne(empty) & pawns : nortOne(empty) & pawns;
}

U64 MoveGenerator::getPawnsAbleToDoublePush(U64 pawns, U64 empty, Color color) {
    U64 doublePushRank = (color == WHITE ? RANK_4 : RANK_5) & empty;
    U64 emptyRank = color == WHITE ? soutOne(doublePushRank) : nortOne(doublePushRank);
    emptyRank &= empty;
    return getPawnsAbleToPush(pawns, emptyRank, color);
}

void MoveGenerator::generatePawnPush(U64 pawns, int& i, Color color, bool isDoublePush) {
    while (pawns) {
        U8 from = bitScanForward(pawns);
        if (from == 0) break;
        U8 to = from + (isDoublePush ? 16 : 8) * (color == WHITE ? 1 : -1);
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
        U64 whitePawns = position.getBitboard(WHITE_PAWN);
        U64 empty = position.getEmptySquares();
        generatePawnPush(getPawnsAbleToPush(whitePawns, empty, WHITE), i, WHITE, false);
        generatePawnPush(getPawnsAbleToDoublePush(whitePawns, empty, WHITE), i, WHITE, true);
    } else {
        U64 blackPawns = position.getBitboard(BLACK_PAWN);
        U64 empty = position.getEmptySquares();
        generatePawnPush(getPawnsAbleToPush(blackPawns, empty, BLACK), i, BLACK, false);
        generatePawnPush(getPawnsAbleToDoublePush(blackPawns, empty, BLACK), i, BLACK, true);
    }
}

// Pawn attacks
void MoveGenerator::precalculatePawnAttacks() {
    for (int i = 0; i < 64; i++) {
        U64 pawn = setBit(0ULL, i);
        arrPawnAttacks[WHITE][i] = noEaOne(pawn) | noWeOne(pawn);
        arrPawnAttacks[BLACK][i] = soEaOne(pawn) | soWeOne(pawn);
    }
}

void MoveGenerator::generatePawnCaptures(Position& position, int& i) {
    Color color = position.getCurrentPlayer();
    U64 pawns = color == WHITE ? position.getBitboard(WHITE_PAWN) : position.getBitboard(BLACK_PAWN);
    U64 enemyPieces = color == WHITE ? position.getBlackOccupiedSquares() : position.getWhiteOccupiedSquares();
    if (position.getEnPassantTarget() != 64) enemyPieces |= 1ULL << position.getEnPassantTarget();
    while (pawns) {
        U8 from = bitScanForward(pawns);
        U64 attacks = arrPawnAttacks[color][from] & enemyPieces;
        while (attacks) {
            U8 to = bitScanForward(attacks);
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
        U64 knight = setBit(0ULL, i);
        U64 l1 = (knight >> 1) & 0x7F7F7F7F7F7F7F7FULL;
        U64 l2 = (knight >> 2) & 0x3F3F3F3F3F3F3F3FULL;
        U64 r1 = (knight << 1) & 0xFEFEFEFEFEFEFEFEULL;
        U64 r2 = (knight << 2) & 0xFCFCFCFCFCFCFCFCULL;
        U64 h1 = l1 | r1;
        U64 h2 = l2 | r2;
        arrKnightMoves[i] = (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
    }
}

void MoveGenerator::generateKnightMoves(Position& position, int& i) {
    Color color = position.getCurrentPlayer();
    U64 knights = color == WHITE ? position.getBitboard(WHITE_KNIGHT) : position.getBitboard(BLACK_KNIGHT);
    U64 friendlyPieces = color == WHITE ? position.getWhiteOccupiedSquares() : position.getBlackOccupiedSquares();
    while (knights) {
        U8 from = bitScanForward(knights);
        U64 moves = arrKnightMoves[from] & ~friendlyPieces;
        while (moves) {
            U8 to = bitScanForward(moves);
            Move move = {from, to, position.getPieceAt(to) == EMPTY ? NORMAL : CAPTURE, position.getPieceAt(to), EMPTY};
            legalMoves[i] = move;
            i++;
            moves &= moves - 1;
        }
        knights &= knights - 1;
    }
}

// KING MOVES
void MoveGenerator::precalculateKingMoves() {
    for (int i = 0; i < 64; i++) {
        U64 kingBitboard = setBit(0ULL, i);
        U64 attacks = eastOne(kingBitboard) | westOne(kingBitboard);
        kingBitboard |= attacks;
        attacks |= nortOne(kingBitboard) | soutOne(kingBitboard);
        arrKingMoves[i] = attacks;
    }
}

void MoveGenerator::generateKingMoves(Position& position, int& i) {
    Color color = position.getCurrentPlayer();
    U64 king = color == WHITE ? position.getBitboard(WHITE_KING) : position.getBitboard(BLACK_KING);
    U64 friendlyPieces = color == WHITE ? position.getWhiteOccupiedSquares() : position.getBlackOccupiedSquares();
    U8 from = bitScanForward(king);
    U64 moves = arrKingMoves[from] & ~friendlyPieces;
    while (moves) {
        U8 to = bitScanForward(moves);
        Move move = {from, to, position.getPieceAt(to) == EMPTY ? NORMAL : CAPTURE, position.getPieceAt(to), EMPTY};
        legalMoves[i] = move;
        i++;
        moves &= moves - 1;
    }
}

// BISHOP MOVES
void MoveGenerator::generateMasks() {
    for (int i = 0; i < 64; i++) {
        bishopRelevantOccupancyMask[i] = maskBishop(i);
        rookRelevantOccupancyMask[i] = maskRook(i);
    }
}

U64 MoveGenerator::maskBishop(int square) {
    U64 mask = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;
    // North East Rays
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) mask = setBit(mask, toBoardIndex(r, f));
    // South East Rays
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) mask = setBit(mask, toBoardIndex(r, f));
    // South West Rays
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) mask = setBit(mask, toBoardIndex(r, f));
    // North West Rays
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) mask = setBit(mask, toBoardIndex(r, f));

    return mask;
}

U64 MoveGenerator::maskRook(int square) {
    U64 mask = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    // North Rays
    for (r = tr + 1; r <= 6; r++) mask = setBit(mask, toBoardIndex(r, tf));
    // South Rays
    for (r = tr - 1; r >= 1; r--) mask = setBit(mask, toBoardIndex(r, tf));
    // East Rays
    for (f = tf + 1; f <= 6; f++) mask = setBit(mask, toBoardIndex(tr, f));
    // West Rays
    for (f = tf - 1; f >= 1; f--) mask = setBit(mask, toBoardIndex(tr, f));

    return mask;
}

U64 MoveGenerator::getBishopAttackOnTheFly(int square, U64 occupancy) {
    U64 attack = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;
    // North East Rays
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }
    // South East Rays
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }
    // South West Rays
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }
    // North West Rays
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }

    return attack;
}