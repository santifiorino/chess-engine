#include "gtest/gtest.h"
#include "Position.h"
#include "MoveGenerator.h"

class MoveGeneratorTest : public ::testing::Test {
protected:
    Position startingPosition = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    MoveGenerator moveGenerator;
};

TEST_F(MoveGeneratorTest, startingPositionPawnMoves) {
    EXPECT_EQ(moveGenerator.generateMoves(startingPosition), 16);
    for (int i = 0; i < 8; i++) {
        Move move = moveGenerator.legalMoves[i];
        EXPECT_EQ(move.from, i + 8);
        EXPECT_EQ(move.to, i + 16);
        EXPECT_EQ(move.type, NORMAL);
        EXPECT_EQ(move.promotion, EMPTY);
        EXPECT_EQ(move.captured, EMPTY);
    }
    for (int i = 8; i < 16; i++) {
        Move move = moveGenerator.legalMoves[i];
        EXPECT_EQ(move.from, i);
        EXPECT_EQ(move.to, i + 16);
        EXPECT_EQ(move.type, NORMAL);
        EXPECT_EQ(move.promotion, EMPTY);
        EXPECT_EQ(move.captured, EMPTY);
    }
}

TEST_F(MoveGeneratorTest, e4BlackToMovePawnMoves) {
    Position position = Position("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(position), 16);
    for (int i = 0; i < 8; i++) {
        Move move = moveGenerator.legalMoves[i];
        EXPECT_EQ(move.from, i + 48);
        EXPECT_EQ(move.to, i + 40);
        EXPECT_EQ(move.type, NORMAL);
        EXPECT_EQ(move.promotion, EMPTY);
        EXPECT_EQ(move.captured, EMPTY);
    }
    for (int i = 8; i < 16; i++) {
        Move move = moveGenerator.legalMoves[i];
        EXPECT_EQ(move.from, i + 40);
        EXPECT_EQ(move.to, i + 24);
        EXPECT_EQ(move.type, NORMAL);
        EXPECT_EQ(move.promotion, EMPTY);
        EXPECT_EQ(move.captured, EMPTY);
    }
}

TEST_F(MoveGeneratorTest, whiteCantPushPawns) {
    Position position = Position("rnbqkbnr/8/8/pppppppp/PPPPPPPP/8/8/RNBQKBNR w KQkq h6 0 9");
    EXPECT_EQ(moveGenerator.generateMoves(position), 0);
}

TEST_F(MoveGeneratorTest, blackCantPushPawns) {
    Position position = Position("rnbqkbnr/8/8/pppppppp/PPPPPPPP/8/3N4/R1BQKBNR b KQkq - 1 9");
    EXPECT_EQ(moveGenerator.generateMoves(position), 0);
}