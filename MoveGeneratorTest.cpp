#include "gtest/gtest.h"
#include "Position.h"
#include "MoveGenerator.h"

class MoveGeneratorTest : public ::testing::Test {
protected:
    Position startingPosition = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    MoveGenerator moveGenerator;
};

TEST_F(MoveGeneratorTest, startingPositionMoves) {
    EXPECT_EQ(moveGenerator.generateMoves(startingPosition), 8);
    for (int i = 0; i < 8; i++) {
        Move move = moveGenerator.legalMoves[i];
        EXPECT_EQ(move.from, i + 8);
        EXPECT_EQ(move.to, i + 16);
        EXPECT_EQ(move.type, NORMAL);
        EXPECT_EQ(move.promotion, EMPTY);
        EXPECT_EQ(move.captured, EMPTY);
    }
}