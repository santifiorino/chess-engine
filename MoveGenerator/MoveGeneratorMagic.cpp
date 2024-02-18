#ifndef MOVEGENERATORMAGIC_CPP
#define MOVEGENERATORMAGIC_CPP

#include "MoveGenerator.h"

void MoveGenerator::precalculateMagicNumbers() {
    for (int i = 0; i < 64; i++) {
        arrBishopMagicNumbers[i] = findMagicNumber(i, BISHOP);
        arrRookMagicNumbers[i] = findMagicNumber(i, ROOK);
    }
}

U64 MoveGenerator::findMagicNumber(int square, PieceType pieceType) {
    U64 occupancies[4096], attacks[4096], usedAttacks[4096];
    RandomNumberGenerator rng = RandomNumberGenerator(1804289383);

    U64 attackMask = (pieceType == BISHOP) ? arrBishopOccupancyMask[square] : arrRookOccupancyMask[square];
    int relevantBits = countOnes(attackMask);

    for (int i = 0; i < (1 << relevantBits); i++) {
        occupancies[i] = placeNumberInMask(i, attackMask);
        attacks[i] = (pieceType == BISHOP) ? bishopMovesMapOnTheFly(square, occupancies[i]) :
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
    // North East - South East - South West - North West
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) mask = setBit(mask, toBoardIndex(r, f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) mask = setBit(mask, toBoardIndex(r, f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) mask = setBit(mask, toBoardIndex(r, f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) mask = setBit(mask, toBoardIndex(r, f));

    return mask;
}

U64 MoveGenerator::rookOccupancyMask(int square) {
    U64 mask = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    // North - South - East - West
    for (r = tr + 1; r <= 6; r++) mask = setBit(mask, toBoardIndex(r, tf));
    for (r = tr - 1; r >= 1; r--) mask = setBit(mask, toBoardIndex(r, tf));
    for (f = tf + 1; f <= 6; f++) mask = setBit(mask, toBoardIndex(tr, f));
    for (f = tf - 1; f >= 1; f--) mask = setBit(mask, toBoardIndex(tr, f));

    return mask;
}

U64 MoveGenerator::placeNumberInMask(int number, U64 mask) {
    U64 occupancy = 0ULL;
    int count = 0;
    while (mask) {
        int square = bitScanForward(mask);
        if (number & (1 << count))
            occupancy |= setBit(0ULL, square);
        mask &= mask - 1;
        count++;
    }
    return occupancy;
};

#endif