#include "gtest/gtest.h"
#include "Position.h"
#include "MoveGenerator.h"

class MoveGeneratorTest : public ::testing::Test {
protected:
    Position startingPosition = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    MoveGenerator moveGenerator;
    void assertEqualMove(Move move, uint8 from, uint8 to, MoveType type, Piece promotion, Piece captured) {
        EXPECT_EQ(move.from, from);
        EXPECT_EQ(move.to, to);
        EXPECT_EQ(move.type, type);
        EXPECT_EQ(move.promotion, promotion);
        EXPECT_EQ(move.captured, captured);
    };
};

TEST_F(MoveGeneratorTest, startingPositionPawnMoves) {
    EXPECT_EQ(moveGenerator.generateMoves(startingPosition), 16);
    for (int i = 0; i < 8; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i+8, i+16, NORMAL, EMPTY, EMPTY);
    }
    for (int i = 8; i < 16; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i, i+16, NORMAL, EMPTY, EMPTY);
    }
}

TEST_F(MoveGeneratorTest, e4BlackToMovePawnMoves) {
    Position position = Position("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(position), 16);
    for (int i = 0; i < 8; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i+48, i+40, NORMAL, EMPTY, EMPTY);
    }
    for (int i = 8; i < 16; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i+40, i+24, NORMAL, EMPTY, EMPTY);
    }
}

TEST_F(MoveGeneratorTest, whiteCantPushPawnsNorCapture) {
    Position position = Position("rn1qkbnr/1b6/p1p1p1p1/PpPpPpPp/1P1P1P1P/8/8/RNBQKBNR w KQkq - 1 13");
    EXPECT_EQ(moveGenerator.generateMoves(position), 0);
}

TEST_F(MoveGeneratorTest, blackCantPushPawnsNorCapture) {
    Position position = Position("rn1qkbnr/1b6/p1p1p1p1/PpPpPpPp/1P1P1P1P/8/3B4/RN1QKBNR b KQkq - 2 13");
    EXPECT_EQ(moveGenerator.generateMoves(position), 0);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMovePosition) {
    Position position = Position("r1bqk2r/ppp4p/n2p1ppn/2bPp3/1PB1P3/2P2N1P/P4PP1/RNBQ1RK1 w kq - 0 10");
    EXPECT_EQ(moveGenerator.generateMoves(position), 7);
    assertEqualMove(moveGenerator.legalMoves[0], 8, 16, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[1], 14, 22, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[2], 23, 31, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[3], 25, 33, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[4], 8, 24, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[5], 14, 30, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[6], 25, 34, CAPTURE, EMPTY, BLACK_BISHOP);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMovePosition) {
    Position position = Position("2kr3r/pppq1p2/2n1bn1p/3pp1p1/B3P3/1P1P1N1P/P1PB1PP1/R2Q1RK1 b - - 0 13");
    EXPECT_EQ(moveGenerator.generateMoves(position), 8);
    assertEqualMove(moveGenerator.legalMoves[0], 35, 27, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[1], 38, 30, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[2], 47, 39, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[3], 48, 40, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[4], 49, 41, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[5], 48, 32, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[6], 49, 33, NORMAL, EMPTY, EMPTY);
    assertEqualMove(moveGenerator.legalMoves[7], 35, 28, CAPTURE, EMPTY, WHITE_PAWN);
}

TEST_F(MoveGeneratorTest, allWhitePawnsCanCapture) {
    Position position = Position("rnbqkbnr/8/8/pppppppp/PPPPPPPP/8/8/RNBQKBNR w KQkq h6 0 9");
    EXPECT_EQ(moveGenerator.generateMoves(position), 14);
    for (int i = 0; i < 7; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i+24, i+24+9, CAPTURE, EMPTY, BLACK_PAWN);
    }
    for (int i = 0; i < 7; i++) {
        Move move = moveGenerator.legalMoves[i+7];
        assertEqualMove(move, i+25, i+25+7, CAPTURE, EMPTY, BLACK_PAWN);
    }

}

TEST_F(MoveGeneratorTest, allBlackPawnsCanCapture) {
    Position position = Position("rnbqkbnr/8/8/pppppppp/PPPPPPPP/8/4K3/RNBQ1BNR b kq - 1 9");
    EXPECT_EQ(moveGenerator.generateMoves(position), 14);
    for (int i = 0; i < 7; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i+32, i+32-7, CAPTURE, EMPTY, WHITE_PAWN);
    }
    for (int i = 0; i < 7; i++) {
        Move move = moveGenerator.legalMoves[i+7];
        assertEqualMove(move, i+33, i+33-9, CAPTURE, EMPTY, WHITE_PAWN);
    }
}

TEST_F(MoveGeneratorTest, whiteEnPassantCapture) {
    Position position = Position("rnbqkbnr/ppp2ppp/8/3Pp3/8/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 3");
    EXPECT_EQ(moveGenerator.generateMoves(position), 16);
    assertEqualMove(moveGenerator.legalMoves[15], D5, E6, EN_PASSANT, EMPTY, BLACK_PAWN);
}

TEST_F(MoveGeneratorTest, blackEnPassantCapture) {
    Position position = Position("rnbqkbnr/ppp2ppp/3P4/8/4pP2/8/PPPP2PP/RNBQKBNR b KQkq f3 0 4");
    EXPECT_EQ(moveGenerator.generateMoves(position), 15);
    assertEqualMove(moveGenerator.legalMoves[13], E4, F3, EN_PASSANT, EMPTY, WHITE_PAWN);
}