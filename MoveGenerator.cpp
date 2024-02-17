#include "MoveGenerator.h"
#include "Position.h"
#include "Utils.h"

MoveGenerator::MoveGenerator() {
    // Pawns
    precalculatePawnAttacks();
    // Knights
    precalculateKnightMoves();
    // King
    precalculateKingMoves();
    // Sliding Pieces
    generateRelevantOccupancyMasks();
    // precalculateMagicNumbers();
    precalculateSliderMoves(true);
    precalculateSliderMoves(false);
}

int MoveGenerator::generateMoves(Position &position) {
    int i = 0;
    // Pawn
    generatePawnPushes(position, i);
    generatePawnCaptures(position, i);
    // Knight
    generateKnightMoves(position, i);
    // King
    generateKingMoves(position, i);
    // Bishop
    generateSliderMoves(position, i, true);
    generateSliderMoves(position, i, false);
    return i;
}

void MoveGenerator::addMove(int& i, U8 from, U8 to, MoveType type, Piece captured, Piece promotion) {
    Move move = {from, to, type, captured, promotion};
    legalMoves[i] = move;
    i++;
}

U64 MoveGenerator::pawnsAbleToPush(U64 pawns, U64 empty, Color color) {
    return color == WHITE ? soutOne(empty) & pawns : nortOne(empty) & pawns;
}

U64 MoveGenerator::pawnsAbleToDoublePush(U64 pawns, U64 empty, Color color) {
    U64 doublePushRank = (color == WHITE ? RANK_4 : RANK_5) & empty;
    U64 emptyRank = color == WHITE ? soutOne(doublePushRank) : nortOne(doublePushRank);
    emptyRank &= empty;
    return pawnsAbleToPush(pawns, emptyRank, color);
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
        generatePawnPush(pawnsAbleToPush(whitePawns, empty, WHITE), i, WHITE, false);
        generatePawnPush(pawnsAbleToDoublePush(whitePawns, empty, WHITE), i, WHITE, true);
    } else {
        U64 blackPawns = position.getBitboard(BLACK_PAWN);
        U64 empty = position.getEmptySquares();
        generatePawnPush(pawnsAbleToPush(blackPawns, empty, BLACK), i, BLACK, false);
        generatePawnPush(pawnsAbleToDoublePush(blackPawns, empty, BLACK), i, BLACK, true);
    }
}

void MoveGenerator::precalculatePawnAttacks() {
    for (int i = 0; i < 64; i++) {
        U64 pawn = setBit(0ULL, i);
        arrPawnAttacks[WHITE][i] = noEaOne(pawn) | noWeOne(pawn);
        arrPawnAttacks[BLACK][i] = soEaOne(pawn) | soWeOne(pawn);
    }
}

void MoveGenerator::generatePawnCaptures(Position& position, int& i) {
    Color color = position.getCurrentPlayer();
    U64 pawns = color == WHITE ? position.getBitboard(WHITE_PAWN) :
                                 position.getBitboard(BLACK_PAWN);
    U64 enemyPieces = color == WHITE ? position.getBlackOccupiedSquares() :
                                       position.getWhiteOccupiedSquares();
    if (position.getEnPassantTarget() != 64) enemyPieces |= 1ULL << position.getEnPassantTarget();
    while (pawns) {
        U8 from = bitScanForward(pawns);
        U64 attacks = arrPawnAttacks[color][from] & enemyPieces;
        while (attacks) {
            U8 to = bitScanForward(attacks);
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
    U64 friendlyPieces = color == WHITE ? position.getWhiteOccupiedSquares() :
                                          position.getBlackOccupiedSquares();
    while (knights) {
        U8 from = bitScanForward(knights);
        U64 moves = arrKnightMoves[from] & ~friendlyPieces;
        while (moves) {
            U8 to = bitScanForward(moves);
            addMove(i, from, to, position.getPieceAt(to) == EMPTY ? NORMAL : CAPTURE, position.getPieceAt(to), EMPTY);
            moves &= moves - 1;
        }
        knights &= knights - 1;
    }
}

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
    U64 friendlyPieces = color == WHITE ? position.getWhiteOccupiedSquares() :
                                          position.getBlackOccupiedSquares();
    U8 from = bitScanForward(king);
    U64 moves = arrKingMoves[from] & ~friendlyPieces;
    while (moves) {
        U8 to = bitScanForward(moves);
        addMove(i, from, to, position.getPieceAt(to) == EMPTY ? NORMAL : CAPTURE, position.getPieceAt(to), EMPTY);
        moves &= moves - 1;
    }
}

void MoveGenerator::generateRelevantOccupancyMasks() {
    for (int i = 0; i < 64; i++) {
        arrBishopOccupancyMask[i] = bishopOccupancyMask(i);
        arrRookOccupancyMask[i] = rookOccupancyMask(i);
    }
}

U64 MoveGenerator::bishopOccupancyMask(int square) {
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

U64 MoveGenerator::rookOccupancyMask(int square) {
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

U64 MoveGenerator::bishopMovesMapOnTheFly(int square, U64 occupancy) {
    U64 attack = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;
    // North East Rays
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }
    // South East Rays
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }
    // South West Rays
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }
    // North West Rays
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attack = setBit(attack, toBoardIndex(r, f));
        if (getBit(occupancy, toBoardIndex(r, f))) break;
    }

    return attack;
}

U64 MoveGenerator::rookMovesMapOnTheFly(int square, U64 occupancy) {
    U64 attack = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    // North Rays
    for (r = tr + 1; r <= 7; r++) {
        attack = setBit(attack, toBoardIndex(r, tf));
        if (getBit(occupancy, toBoardIndex(r, tf))) break;
    }
    // South Rays
    for (r = tr - 1; r >= 0; r--) {
        attack = setBit(attack, toBoardIndex(r, tf));
        if (getBit(occupancy, toBoardIndex(r, tf))) break;
    }
    // East Rays
    for (f = tf + 1; f <= 7; f++) {
        attack = setBit(attack, toBoardIndex(tr, f));
        if (getBit(occupancy, toBoardIndex(tr, f))) break;
    }
    // West Rays
    for (f = tf - 1; f >= 0; f--) {
        attack = setBit(attack, toBoardIndex(tr, f));
        if (getBit(occupancy, toBoardIndex(tr, f))) break;
    }

    return attack;
}

U64 MoveGenerator::getOccupancyByIndex(int index, U64 mask) {
    U64 occupancy = 0ULL;
    int count = 0;
    while (mask) {
        int square = bitScanForward(mask);
        if (index & (1 << count))
            occupancy |= setBit(0ULL, square);
        mask &= mask - 1;
        count++;
    }
    return occupancy;
};

U64 MoveGenerator::findMagicNumber(int square, bool isBishop) {
    U64 occupancies[4096], attacks[4096], usedAttacks[4096];
    RandomNumberGenerator rng = RandomNumberGenerator(1804289383);

    U64 attackMask = isBishop ? arrBishopOccupancyMask[square] : arrRookOccupancyMask[square];
    int relevantBits = countOnes(attackMask);

    for (int i = 0; i < (1 << relevantBits); i++) {
        occupancies[i] = getOccupancyByIndex(i, attackMask);
        attacks[i] = isBishop ? bishopMovesMapOnTheFly(square, occupancies[i]) :
                                  rookMovesMapOnTheFly(square, occupancies[i]);
    }
    for (int iterations = 0; iterations < 100000000; iterations++) {
        U64 magic = rng.generateRandomU64FewBits();
        if (countOnes((attackMask * magic) & 0xFF00000000000000ULL) < 6) continue;
        int i, fail;
        for (i = 0; i < 4096; i++) usedAttacks[i] = 0ULL;
        for (i = 0, fail = 0; !fail && i < (1 << relevantBits); i++) {
            int magicIndex = (int) ((occupancies[i] * magic) >> (64 - relevantBits));
            if (usedAttacks[magicIndex] == 0ULL) usedAttacks[magicIndex] = attacks[i];
            else if (usedAttacks[magicIndex] != attacks[i]) fail = 1;
        }
        if (!fail) return magic;
    }
    return 0ULL;
}

void MoveGenerator::precalculateMagicNumbers() {
    for (int i = 0; i < 64; i++) {
        arrBishopMagicNumbers[i] = findMagicNumber(i, true);
        arrRookMagicNumbers[i] = findMagicNumber(i, false);
    }
}

void MoveGenerator::precalculateSliderMoves(bool isBishop) {
    for (int i = 0; i < 64; i++) {
        U64 attackMask = isBishop ? arrBishopOccupancyMask[i] : arrRookOccupancyMask[i];
        int relevantBitsCount = isBishop ? arrBishopRelevantBits[i] : arrRookRelevantBits[i];
        for (int index = 0; index < 1 << relevantBitsCount; index++) {
            U64 occupancy = getOccupancyByIndex(index, attackMask);
            if (isBishop) {
                int magicIndex = (int) ((occupancy * arrBishopMagicNumbers[i]) >> (64 - relevantBitsCount));
                arrBishopMoves[i][magicIndex] = bishopMovesMapOnTheFly(i, occupancy);
            } else {
                int magicIndex = (int) ((occupancy * arrRookMagicNumbers[i]) >> (64 - relevantBitsCount));
                arrRookMoves[i][magicIndex] = rookMovesMapOnTheFly(i, occupancy);
            }
        }
    }
}

void MoveGenerator::generateSliderMoves(Position& position, int& i, bool isBishop) {
    Color color = position.getCurrentPlayer();
    U64 pieces;
    if (isBishop) {
        pieces = color == WHITE ? position.getBitboard(WHITE_BISHOP) :
                                  position.getBitboard(BLACK_BISHOP);
    } else {
        pieces = color == WHITE ? position.getBitboard(WHITE_ROOK) :
                                  position.getBitboard(BLACK_ROOK);
    }
    U64 friendlyPieces = color == WHITE ? position.getWhiteOccupiedSquares() :
                                          position.getBlackOccupiedSquares();
    while (pieces) {
        U8 from = bitScanForward(pieces);
        U64 occupancy = position.getOccupiedSquares();
        occupancy &= isBishop ? arrBishopOccupancyMask[from] : arrRookOccupancyMask[from];
        int magicIndex;
        U64 moves;
        if (isBishop) {
            magicIndex = (int) ((occupancy * arrBishopMagicNumbers[from]) >> (64 - arrBishopRelevantBits[from]));
            moves = arrBishopMoves[from][magicIndex] & ~friendlyPieces;
        } else {
            magicIndex = (int) ((occupancy * arrRookMagicNumbers[from]) >> (64 - arrRookRelevantBits[from]));
            moves = arrRookMoves[from][magicIndex] & ~friendlyPieces;
        }
        while (moves) {
            U8 to = bitScanForward(moves);
            addMove(i, from, to, position.getPieceAt(to) == EMPTY ? NORMAL : CAPTURE, position.getPieceAt(to), EMPTY);
            moves &= moves - 1;
        }
        pieces &= pieces - 1;
    }
}

