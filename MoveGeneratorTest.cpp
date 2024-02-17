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
    void assertEqualMove(Move move, U8 from, U8 to, MoveType type, Piece captured, Piece promotion) {
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
    EXPECT_EQ(moveGenerator.generateMoves(arbitraryWhiteToMovePosition), 18);
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

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveKingMoves) {
    moveGenerator.generateMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[16], G1, H1, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[17], G1, H2, NORMAL, EMPTY, EMPTY);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveAmountOfMoves) {
    EXPECT_EQ(moveGenerator.generateMoves(arbitraryBlackToMovePosition), 20);
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

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveKingMoves) {
    moveGenerator.generateMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[19], C8, B8, NORMAL, EMPTY, EMPTY);
}

TEST_F(MoveGeneratorTest, whiteEnPassantLeftToRight) {
    Position whiteEnPassantLeftToRightPosition = Position("r1b1kb1r/2qn3p/p1n5/1pppPppP/3p4/NBP2N2/PP3PP1/R1BQ1RK1 w kq f6 0 15");
    EXPECT_EQ(moveGenerator.generateMoves(whiteEnPassantLeftToRightPosition), 19);
    assertEqualMove(moveGenerator.legalMoves[6], E5, F6, EN_PASSANT, BLACK_PAWN, EMPTY);
}

TEST_F(MoveGeneratorTest, whiteEnPassantRighToLeft) {
    Position whiteEnPassantRightToLeftPosition = Position("r1b1kb1r/2qn1p1p/p1np4/1pp3pP/3pP3/N1P2N2/PPB2PP1/R1BQ1RK1 w kq g6 0 13");
    EXPECT_EQ(moveGenerator.generateMoves(whiteEnPassantRightToLeftPosition), 21);
    assertEqualMove(moveGenerator.legalMoves[8], H5, G6, EN_PASSANT, BLACK_PAWN, EMPTY);
}


TEST_F(MoveGeneratorTest, blackEnPassantLeftToRight) {
    Position blackEnPassantLeftToRightPosition = Position("r1b1kb1r/pp3ppp/1qnp1n2/1B2p3/2pPP3/N1P2N1P/PP3PP1/R1BQ1RK1 b kq d3 0 8");
    EXPECT_EQ(moveGenerator.generateMoves(blackEnPassantLeftToRightPosition), 24);
    assertEqualMove(moveGenerator.legalMoves[7], C4, D3, EN_PASSANT, WHITE_PAWN, EMPTY);
}

TEST_F(MoveGeneratorTest, blackEnPassantRightToLeft) {
    Position blackEnPassantRightToLeftPosition = Position("r1b1kb1r/pp3ppp/1qnp1n2/1B2p3/1Pp1P3/N1P2N1P/P2P1PP1/R1BQ1RK1 b kq b3 0 8");
    EXPECT_EQ(moveGenerator.generateMoves(blackEnPassantRightToLeftPosition), 23);
    assertEqualMove(moveGenerator.legalMoves[7], C4, B3, EN_PASSANT, WHITE_PAWN, EMPTY);
}

TEST_F(MoveGeneratorTest, whitePawnPromotion) {
    Position whitePromotePosition = Position("r7/1kn2P2/1q6/8/2p5/4B3/6K1/5R2 w - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(whitePromotePosition), 11);
    assertEqualMove(moveGenerator.legalMoves[0], F7, F8, PROMOTION, EMPTY, WHITE_QUEEN);
    assertEqualMove(moveGenerator.legalMoves[1], F7, F8, PROMOTION, EMPTY, WHITE_ROOK);
    assertEqualMove(moveGenerator.legalMoves[2], F7, F8, PROMOTION, EMPTY, WHITE_BISHOP);
    assertEqualMove(moveGenerator.legalMoves[3], F7, F8, PROMOTION, EMPTY, WHITE_KNIGHT);
}

TEST_F(MoveGeneratorTest, whitePawnCapturePromotion) {
    Position whiteCapturePromotePosition = Position("r3r1b1/1kn2P2/nq6/5Q2/2p5/4B3/6K1/5R2 w - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(whiteCapturePromotePosition), 19);
    assertEqualMove(moveGenerator.legalMoves[0], F7, F8, PROMOTION, EMPTY, WHITE_QUEEN);
    assertEqualMove(moveGenerator.legalMoves[1], F7, F8, PROMOTION, EMPTY, WHITE_ROOK);
    assertEqualMove(moveGenerator.legalMoves[2], F7, F8, PROMOTION, EMPTY, WHITE_BISHOP);
    assertEqualMove(moveGenerator.legalMoves[3], F7, F8, PROMOTION, EMPTY, WHITE_KNIGHT);

    assertEqualMove(moveGenerator.legalMoves[4], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, WHITE_QUEEN);
    assertEqualMove(moveGenerator.legalMoves[5], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, WHITE_ROOK);
    assertEqualMove(moveGenerator.legalMoves[6], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, WHITE_BISHOP);
    assertEqualMove(moveGenerator.legalMoves[7], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, WHITE_KNIGHT);

    assertEqualMove(moveGenerator.legalMoves[8], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, WHITE_QUEEN);
    assertEqualMove(moveGenerator.legalMoves[9], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, WHITE_ROOK);
    assertEqualMove(moveGenerator.legalMoves[10], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, WHITE_BISHOP);
    assertEqualMove(moveGenerator.legalMoves[11], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, WHITE_KNIGHT);
};

TEST_F(MoveGeneratorTest, blackPawnPromotion) {
    Position blackToPromotePosition = Position("8/1k6/8/8/8/8/1p4K1/8 b - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(blackToPromotePosition), 12);
    assertEqualMove(moveGenerator.legalMoves[0], B2, B1, PROMOTION, EMPTY, BLACK_QUEEN);
    assertEqualMove(moveGenerator.legalMoves[1], B2, B1, PROMOTION, EMPTY, BLACK_ROOK);
    assertEqualMove(moveGenerator.legalMoves[2], B2, B1, PROMOTION, EMPTY, BLACK_BISHOP);
    assertEqualMove(moveGenerator.legalMoves[3], B2, B1, PROMOTION, EMPTY, BLACK_KNIGHT);
}

TEST_F(MoveGeneratorTest, blackPawnCapturePromotion) {
    Position blackToCapturePromotePosition = Position("8/1k6/8/8/8/8/1p4K1/R1B5 b - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(blackToCapturePromotePosition), 20);
    assertEqualMove(moveGenerator.legalMoves[0], B2, B1, PROMOTION, EMPTY, BLACK_QUEEN);
    assertEqualMove(moveGenerator.legalMoves[1], B2, B1, PROMOTION, EMPTY, BLACK_ROOK);
    assertEqualMove(moveGenerator.legalMoves[2], B2, B1, PROMOTION, EMPTY, BLACK_BISHOP);
    assertEqualMove(moveGenerator.legalMoves[3], B2, B1, PROMOTION, EMPTY, BLACK_KNIGHT);

    assertEqualMove(moveGenerator.legalMoves[4], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, BLACK_QUEEN);
    assertEqualMove(moveGenerator.legalMoves[5], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, BLACK_ROOK);
    assertEqualMove(moveGenerator.legalMoves[6], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, BLACK_BISHOP);
    assertEqualMove(moveGenerator.legalMoves[7], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, BLACK_KNIGHT);

    assertEqualMove(moveGenerator.legalMoves[8], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, BLACK_QUEEN);
    assertEqualMove(moveGenerator.legalMoves[9], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, BLACK_ROOK);
    assertEqualMove(moveGenerator.legalMoves[10], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, BLACK_BISHOP);
    assertEqualMove(moveGenerator.legalMoves[11], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, BLACK_KNIGHT);
}

TEST_F(MoveGeneratorTest, whiteKingMovesAndCaptures) {
    Position whiteKingCapturePosition = Position("8/1n6/1k6/8/4bp2/5K2/4NP2/8 w - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(whiteKingCapturePosition), 12);
    assertEqualMove(moveGenerator.legalMoves[6], F3, G2, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[7], F3, E3, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[8], F3, G3, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[9], F3, E4, CAPTURE, BLACK_BISHOP, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[10], F3, F4, CAPTURE, BLACK_PAWN, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[11], F3, G4, NORMAL, EMPTY, EMPTY);
}

TEST_F(MoveGeneratorTest, blackKingMovesAndCaptures) {
    Position blackKingCapturePosition = Position("4r3/4kp2/3PnB2/8/2Q5/1K6/8/8 b - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(blackKingCapturePosition), 13);
    assertEqualMove(moveGenerator.legalMoves[8], E7, D6, CAPTURE, WHITE_PAWN, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[9], E7, F6, CAPTURE, WHITE_BISHOP, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[10], E7, D7, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[11], E7, D8, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[12], E7, F8, NORMAL, EMPTY, EMPTY);
}