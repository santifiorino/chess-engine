#include "gtest/gtest.h"
#include "Position.h"

class PositionTest : public ::testing::Test {
protected:
    Position startingPosition = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

};

TEST_F(PositionTest, StartingPositionBitBoard) {
    // 1111 1111 | 1 rank
    EXPECT_EQ(startingPosition.getFriendlyPieces(PAWN), 0xFF00);
    // 0100 0010
    EXPECT_EQ(startingPosition.getFriendlyPieces(KNIGHT), 0x42);
    // 0010 0100
    EXPECT_EQ(startingPosition.getFriendlyPieces(BISHOP), 0x24);
    // 1000 0001
    EXPECT_EQ(startingPosition.getFriendlyPieces(ROOK), 0x81);
    // 0000 1000
    EXPECT_EQ(startingPosition.getFriendlyPieces(QUEEN), 0x8);
    // 0001 0000
    EXPECT_EQ(startingPosition.getFriendlyPieces(KING), 0x10);
    // 1111 1111 | 6 ranks
    EXPECT_EQ(startingPosition.getEnemyPieces(PAWN), 0xFF000000000000);
    // 0100 0010 | 7 ranks
    EXPECT_EQ(startingPosition.getEnemyPieces(KNIGHT), 0x4200000000000000);
    // 0010 0100 | 7 ranks
    EXPECT_EQ(startingPosition.getEnemyPieces(BISHOP), 0x2400000000000000);
    // 1000 0001 | 7 ranks
    EXPECT_EQ(startingPosition.getEnemyPieces(ROOK), 0x8100000000000000);
    // 0000 1000 | 7 ranks
    EXPECT_EQ(startingPosition.getEnemyPieces(QUEEN), 0x800000000000000);
    // 0001 0000 | 7 ranks
    EXPECT_EQ(startingPosition.getEnemyPieces(KING), 0x1000000000000000);
}

TEST_F(PositionTest, StartingPositionEmptySquaresBitboard) {
    // 1111 1111 1111 1111 | 4 ranks | 1111 1111 1111 1111
    EXPECT_EQ(startingPosition.getEmptySquares(), 0xFFFFFFFF0000);
}

TEST_F(PositionTest, StartingPositionPlayer) {
    EXPECT_EQ(startingPosition.getCurrentPlayer(), WHITE);
}

TEST_F(PositionTest, StartingPositionCastlingRights) {
    EXPECT_EQ(startingPosition.whiteCanKingsideCastle(), true);
    EXPECT_EQ(startingPosition.whiteCanQueensideCastle(), true);
    EXPECT_EQ(startingPosition.blackCanKingsideCastle(), true);
    EXPECT_EQ(startingPosition.blackCanQueensideCastle(), true);
}

TEST_F(PositionTest, StartingPositionEnPassantTarget) {
    EXPECT_EQ(startingPosition.getEnPassantTarget(), 64);
}

TEST_F(PositionTest, EnPassantPositionEnPassantTarget) {
    Position enPassantPosition = Position("rnbqkbnr/ppp2ppp/8/3Pp3/8/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 3");
    EXPECT_EQ(enPassantPosition.getEnPassantTarget(), E6);
}

TEST_F(PositionTest, StartingPositionHalfmoveClock) {
    EXPECT_EQ(startingPosition.getHalfmoveClock(), 0);
}

TEST_F(PositionTest, MultipleDigitsHalfmoveClock) {
    Position position = Position("rnbqkb1r/pppp1ppp/5n2/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 18 11");
    EXPECT_EQ(position.getHalfmoveClock(), 18);
}

TEST_F(PositionTest, StartingPositionFullmoveCounter) {
    EXPECT_EQ(startingPosition.getFullmoveCounter(), 1);
}

TEST_F(PositionTest, MultipleDigitsFullmoveCounter) {
    Position position = Position("rnbqkb1r/pppp1ppp/5n2/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 18 11");
    EXPECT_EQ(position.getFullmoveCounter(), 11) << "Expected fullmove counter to be 11";
}