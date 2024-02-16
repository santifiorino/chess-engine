#include "gtest/gtest.h"
#include "Position.h"
#include "MoveGenerator.h"

class MoveGeneratorTest : public ::testing::Test {
protected:
    Position startingPosition = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Position e4BlackToMovePosition = Position("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    Position arbitraryWhiteToMovePosition = Position("r1bqk2r/ppp4p/n2p1ppn/2bPp3/1PB1P3/2P2N1P/P4PP1/RNBQ1RK1 w kq - 0 10");
    Position arbitraryBlackToMovePosition = Position("2kr3r/pppq1p2/2n1bn1p/3pp1p1/B3P3/1P1P1N1P/P1PB1PP1/R2Q1RK1 b - - 0 13");
    MoveGenerator moveGenerator;
    void assertEqualMove(Move move, uint8 from, uint8 to, MoveType type, Piece captured, Piece promotion) {
        EXPECT_EQ(move.from, from);
        EXPECT_EQ(move.to, to);
        EXPECT_EQ(move.type, type);
        EXPECT_EQ(move.captured, captured);
        EXPECT_EQ(move.promotion, promotion);
    };
};

TEST_F(MoveGeneratorTest, startingPositionPawnMoves) {
    moveGenerator.generateMoves(startingPosition);
    for (int i = 0; i < 8; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i+8, i+16, NORMAL, EMPTY, EMPTY);
    }
    for (int i = 8; i < 16; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i, i+16, NORMAL, EMPTY, EMPTY);
    }
}

TEST_F(MoveGeneratorTest, startingPositionKnightMoves) {
    moveGenerator.generateMoves(startingPosition);
    assertEqualMove(moveGenerator.legalMoves[16], B1, A3, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[17], B1, C3, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[18], G1, F3, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[19], G1, H3, NORMAL, EMPTY, EMPTY);
}

TEST_F(MoveGeneratorTest, startingPositionAmoutOfMoves) {
    moveGenerator.generateMoves(startingPosition);
    EXPECT_EQ(moveGenerator.generateMoves(startingPosition), 20);
}

TEST_F(MoveGeneratorTest, e4BlackToMoveAmountOfMoves) {
    EXPECT_EQ(moveGenerator.generateMoves(e4BlackToMovePosition), 20);
}

TEST_F(MoveGeneratorTest, e4BlackToMovePawnMoves) {
    moveGenerator.generateMoves(e4BlackToMovePosition);
    for (int i = 0; i < 8; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i+48, i+40, NORMAL, EMPTY, EMPTY);
    }
    for (int i = 8; i < 16; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i+40, i+24, NORMAL, EMPTY, EMPTY);
    }
}

TEST_F(MoveGeneratorTest, e4BlackToMoveKnightMoves) {
    moveGenerator.generateMoves(e4BlackToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[16], B8, A6, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[17], B8, C6, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[18], G8, F6, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[19], G8, H6, NORMAL, EMPTY, EMPTY);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveAmountOfMoves) {
    EXPECT_EQ(moveGenerator.generateMoves(arbitraryWhiteToMovePosition), 16);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMovePawnMoves) {
    moveGenerator.generateMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[0], A2, A3, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[1], G2, G3, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[2], H3, H4, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[3], B4, B5, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[4], A2, A4, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[5], G2, G4, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[6], B4, C5, CAPTURE, BLACK_BISHOP, EMPTY);
}

TEST_F(MoveGeneratorTest, arbitrariWhiteToMoveKnightMoves) {
    moveGenerator.generateMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[7], B1, D2, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[8], B1, A3, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[9], F3, E1, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[10], F3, D2, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[11], F3, H2, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[12], F3, D4, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[13], F3, H4, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[14], F3, E5, CAPTURE, BLACK_PAWN, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[15], F3, G5, NORMAL, EMPTY, EMPTY);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveAmountOfMoves) {
    EXPECT_EQ(moveGenerator.generateMoves(arbitraryBlackToMovePosition), 19);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMovePawnMoves) {
    moveGenerator.generateMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[0], 35, 27, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[1], 38, 30, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[2], 47, 39, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[3], 48, 40, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[4], 49, 41, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[5], 48, 32, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[6], 49, 33, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[7], 35, 28, CAPTURE, WHITE_PAWN, EMPTY);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveKnightMoves) {
    moveGenerator.generateMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[8], C6, B4, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[9], C6, D4, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[10], C6, A5, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[11], C6, E7, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[12], C6, B8, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[13], F6, E4, CAPTURE, WHITE_PAWN, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[14], F6, G4, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[15], F6, H5, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[16], F6, H7, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[17], F6, E8, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[18], F6, G8, NORMAL, EMPTY, EMPTY);
}

TEST_F(MoveGeneratorTest, whiteEnPassantLeftToRight) {
    Position whiteEnPassantLeftToRightPosition = Position("r1b1kb1r/2qn3p/p1n5/1pppPppP/3p4/NBP2N2/PP3PP1/R1BQ1RK1 w kq f6 0 15");
    EXPECT_EQ(moveGenerator.generateMoves(whiteEnPassantLeftToRightPosition), 17);
    assertEqualMove(moveGenerator.legalMoves[6], E5, F6, EN_PASSANT, BLACK_PAWN, EMPTY);
}

TEST_F(MoveGeneratorTest, whiteEnPassantRighToLeft) {
    Position whiteEnPassantRightToLeftPosition = Position("r1b1kb1r/2qn1p1p/p1np4/1pp3pP/3pP3/N1P2N2/PPB2PP1/R1BQ1RK1 w kq g6 0 13");
    EXPECT_EQ(moveGenerator.generateMoves(whiteEnPassantRightToLeftPosition), 19);
    assertEqualMove(moveGenerator.legalMoves[8], H5, G6, EN_PASSANT, BLACK_PAWN, EMPTY);
}


TEST_F(MoveGeneratorTest, blackEnPassantLeftToRight) {
    Position blackEnPassantLeftToRightPosition = Position("r1b1kb1r/pp3ppp/1qnp1n2/1B2p3/2pPP3/N1P2N1P/PP3PP1/R1BQ1RK1 b kq d3 0 8");
    EXPECT_EQ(moveGenerator.generateMoves(blackEnPassantLeftToRightPosition), 21);
    assertEqualMove(moveGenerator.legalMoves[7], C4, D3, EN_PASSANT, WHITE_PAWN, EMPTY);
}

TEST_F(MoveGeneratorTest, blackEnPassantRightToLeft) {
    Position blackEnPassantRightToLeftPosition = Position("r1b1kb1r/pp3ppp/1qnp1n2/1B2p3/1Pp1P3/N1P2N1P/P2P1PP1/R1BQ1RK1 b kq b3 0 8");
    EXPECT_EQ(moveGenerator.generateMoves(blackEnPassantRightToLeftPosition), 20);
    assertEqualMove(moveGenerator.legalMoves[7], C4, B3, EN_PASSANT, WHITE_PAWN, EMPTY);
}