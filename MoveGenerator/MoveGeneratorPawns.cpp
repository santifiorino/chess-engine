#ifndef MOVEGENERATORPAWNS_CPP
#define MOVEGENERATORPAWNS_CPP

#include "MoveGenerator.h"

void MoveGenerator::precalculatePawnAttacks() {
    for (int i = 0; i < 64; i++) {
        U64 pawn = setBit(0ULL, i);
        arrPawnAttacks[WHITE][i] = noEaOne(pawn) | noWeOne(pawn);
        arrPawnAttacks[BLACK][i] = soEaOne(pawn) | soWeOne(pawn);
    }
}

void MoveGenerator::generatePawnPushes(Position& position, int& i){
    Color color = position.getCurrentPlayer();
    U64 pawns = position.getFriendlyPieces(PAWN);
    U64 empty = position.getEmptySquares();
    generatePawnPush(pawnsAbleToPush(pawns, empty, color), i, color, false);
    generatePawnPush(pawnsAbleToDoublePush(pawns, empty, color), i, color, true);
}

void MoveGenerator::generatePawnPush(U64 pawns, int& i, Color color, bool isDoublePush) {
    while (pawns) {
        U8 from = bitScanForward(pawns);
        if (from == 0) break;
        U8 to = from + (isDoublePush ? 16 : 8) * (color == WHITE ? 1 : -1);
        if (color == WHITE && to >= 56 || color == BLACK && to < 8) {
            if (color == WHITE) {
                addMove(i, from, to, PROMOTION, NOPIECE, WHITE_QUEEN);
                addMove(i, from, to, PROMOTION, NOPIECE, WHITE_ROOK);
                addMove(i, from, to, PROMOTION, NOPIECE, WHITE_BISHOP);
                addMove(i, from, to, PROMOTION, NOPIECE, WHITE_KNIGHT);
            } else {
                addMove(i, from, to, PROMOTION, NOPIECE, BLACK_QUEEN);
                addMove(i, from, to, PROMOTION, NOPIECE, BLACK_ROOK);
                addMove(i, from, to, PROMOTION, NOPIECE, BLACK_BISHOP);
                addMove(i, from, to, PROMOTION, NOPIECE, BLACK_KNIGHT);
            }
            
        } else {
            addMove(i, from, to, isDoublePush ? DOUBLE_PAWN_PUSH : NORMAL, NOPIECE, NOPIECE);
        }
        pawns &= pawns - 1;
    }
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

void MoveGenerator::generatePawnCaptures(Position& position, int& i) {
    Color color = position.getCurrentPlayer();
    U64 pawns = position.getFriendlyPieces(PAWN);
    U64 enemyPieces = position.getEnemyPieces();
    if (position.getEnPassantTarget() != NOSQUARE) enemyPieces |= 1ULL << position.getEnPassantTarget();
    while (pawns) {
        U8 from = bitScanForward(pawns);
        U64 attacks = arrPawnAttacks[color][from] & enemyPieces;
        while (attacks) {
            U8 to = bitScanForward(attacks);
            if (to == position.getEnPassantTarget()) {
                addMove(i, from, to, EN_PASSANT, color == WHITE ? BLACK_PAWN : WHITE_PAWN, NOPIECE);
            } else {
                if (color == WHITE && to >= 56 || color == BLACK && to < 8) {
                    if (color == WHITE) {
                        addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), WHITE_QUEEN);
                        addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), WHITE_ROOK);
                        addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), WHITE_BISHOP);
                        addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), WHITE_KNIGHT);
                    } else {
                        addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), BLACK_QUEEN);
                        addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), BLACK_ROOK);
                        addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), BLACK_BISHOP);
                        addMove(i, from, to, PROMOTION_CAPTURE, position.getPieceAt(to), BLACK_KNIGHT);
                    }
                } else {
                    addMove(i, from, to, CAPTURE, position.getPieceAt(to), NOPIECE);
                }
            }
            attacks &= attacks - 1;
        }
        pawns &= pawns - 1;
    }
}

#endif