#include "gtest/gtest.h"
#include "../ChessGame/ChessGame.h"

class ChessGameTest : public ::testing::Test {
protected:
    ChessGame game = ChessGame();
};

TEST_F(ChessGameTest, whiteKingUnderCheckLegalMoves) {
    game.makeMove(D2, D4);
    game.makeMove(E7, E5);
    game.makeMove(E2, E4);
    game.makeMove(F8, B4);
    ASSERT_EQ(game.getMovesFrom(B1), setBit(setBit(0, C3), D2));
    ASSERT_EQ(game.getMovesFrom(C1), setBit(0, D2));
    ASSERT_EQ(game.getMovesFrom(D1), setBit(0, D2));
    ASSERT_EQ(game.getMovesFrom(E1), setBit(0, E2));
    ASSERT_EQ(game.getMovesFrom(F1), 0);
    ASSERT_EQ(game.getMovesFrom(G1), 0);
    ASSERT_EQ(game.getMovesFrom(A2), 0);
    ASSERT_EQ(game.getMovesFrom(B2), 0);
    ASSERT_EQ(game.getMovesFrom(C2), setBit(0, C3));
    ASSERT_EQ(game.getMovesFrom(F2), 0);
    ASSERT_EQ(game.getMovesFrom(G2), 0);
    ASSERT_EQ(game.getMovesFrom(H2), 0);
    ASSERT_EQ(game.getMovesFrom(D4), 0);
}

// TODO: Test checks, checkmates, stalemates, en passant checks, etc. 