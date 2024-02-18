#include "gtest/gtest.h"
#include "Position.h"

class PositionTest : public ::testing::Test {
protected:
    Position startingPosition = Position();
    Position arbitraryWhiteToMovePosition = Position();

    void SetUp() override {
        startingPosition.parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        arbitraryWhiteToMovePosition.parseFen("r1bqk2r/ppp4p/n2p1ppn/2bPp3/1PB1P3/2P2N1P/P4PP1/RNBQ1RK1 w kq - 0 10");
    }
};

TEST_F(PositionTest, StartingPositionBitboard) {
    // 1111 1111 | 1 rank
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_PAWN), 0xFF00);
    // 0100 0010
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_KNIGHT), 0x42);
    // 0010 0100
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_BISHOP), 0x24);
    // 1000 0001
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_ROOK), 0x81);
    // 0000 1000
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_QUEEN), 0x8);
    // 0001 0000
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_KING), 0x10);
    // 1111 1111 | 6 ranks
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_PAWN), 0xFF000000000000);
    // 0100 0010 | 7 ranks
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_KNIGHT), 0x4200000000000000);
    // 0010 0100 | 7 ranks
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_BISHOP), 0x2400000000000000);
    // 1000 0001 | 7 ranks
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_ROOK), 0x8100000000000000);
    // 0000 1000 | 7 ranks
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_QUEEN), 0x800000000000000);
    // 0001 0000 | 7 ranks
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_KING), 0x1000000000000000);
}

TEST_F(PositionTest, arbitraryWhiteToMoveBitboard) {
    // 1000 0001 0010 1000 0100 0110 0001 | 1 rank
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(WHITE_PAWN), 0x812846100);
    // 0010 0000 0000 0000 0000 0010
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(WHITE_KNIGHT), 0x200002);
    // 0100 0000 0000 0000 0000 0000 0100
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(WHITE_BISHOP), 0x4000004);
    // 0010 0001
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(WHITE_ROOK), 0x21);
    // 1000
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(WHITE_QUEEN), 0x8);
    // 0100 0000
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(WHITE_KING), 0x40);
    // 1000 0111 0110 1000 0001 0000 | 4 ranks
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(BLACK_PAWN), 0x87681000000000);
    // 1000 0001 | 5 ranks
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(BLACK_KNIGHT), 0x810000000000);
    // 0100 | 2 ranks | 0000 0100 | 4 ranks
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(BLACK_BISHOP), 0x400000400000000);
    // 1000 0001 | 7 ranks
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(BLACK_ROOK), 0x8100000000000000);
    // 1000 | 7 ranks
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(BLACK_QUEEN), 0x800000000000000);
    // 0001 0000 | 7 ranks
    EXPECT_EQ(arbitraryWhiteToMovePosition.getOccupiedSquares(BLACK_KING), 0x1000000000000000);
}

TEST_F(PositionTest, StartingPositionEmptySquaresBitboard) {
    // 1111 1111 1111 1111 | 4 ranks | 1111 1111 1111 1111
    EXPECT_EQ(startingPosition.getEmptySquares(), 0xFFFFFFFF0000);
}

TEST_F(PositionTest, StartingPositionPlayer) {
    EXPECT_EQ(startingPosition.getCurrentPlayer(), WHITE);
}

TEST_F(PositionTest, StartingPositionCastlingRights) {
    EXPECT_TRUE(startingPosition.whiteCanKingsideCastle());
    EXPECT_TRUE(startingPosition.whiteCanQueensideCastle());
    EXPECT_TRUE(startingPosition.blackCanKingsideCastle());
    EXPECT_TRUE(startingPosition.blackCanQueensideCastle());
}

TEST_F(PositionTest, StartingPositionEnPassantTarget) {
    EXPECT_EQ(startingPosition.getEnPassantTarget(), NOSQUARE);
}

TEST_F(PositionTest, EnPassantPositionEnPassantTarget) {
    Position enPassantPosition = Position();
    enPassantPosition.parseFen("rnbqkbnr/ppp2ppp/8/3Pp3/8/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 3");
    EXPECT_EQ(enPassantPosition.getEnPassantTarget(), E6);
}

TEST_F(PositionTest, StartingPositionHalfmoveClock) {
    EXPECT_EQ(startingPosition.getHalfmoveClock(), 0);
}

TEST_F(PositionTest, MultipleDigitsHalfmoveClock) {
    Position position = Position();
    position.parseFen("rnbqkb1r/pppp1ppp/5n2/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 18 11");
    EXPECT_EQ(position.getHalfmoveClock(), 18);
}

TEST_F(PositionTest, StartingPositionFullmoveCounter) {
    EXPECT_EQ(startingPosition.getFullmoveCounter(), 1);
}

TEST_F(PositionTest, MultipleDigitsFullmoveCounter) {
    Position position = Position();
    position.parseFen("rnbqkb1r/pppp1ppp/5n2/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 18 11");
    EXPECT_EQ(position.getFullmoveCounter(), 11) << "Expected fullmove counter to be 11";
}

TEST_F(PositionTest, StartingPositionPlaysE4) {
    startingPosition.makeMove({E2, E4, NORMAL, NOPIECE, NOTYPE});
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_PAWN), 0x1000EF00);

    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_KNIGHT), 0x42);
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_BISHOP), 0x24);
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_ROOK), 0x81);
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_QUEEN), 0x8);
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_KING), 0x10);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_PAWN), 0xFF000000000000);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_KNIGHT), 0x4200000000000000);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_BISHOP), 0x2400000000000000);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_ROOK), 0x8100000000000000);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_QUEEN), 0x800000000000000);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_KING), 0x1000000000000000);
}

TEST_F(PositionTest, WhitePawnCapture) {
    startingPosition.makeMove({E2, E4, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({D7, D5, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({E4, D5, CAPTURE, BLACK_PAWN, NOTYPE});
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_PAWN), 0x80000EF00);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_PAWN), 0xF7000000000000);
}

TEST_F(PositionTest, WhiteKnightCapture) {
    startingPosition.makeMove({G1, F3, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({B8, C6, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({B1, C3, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({C6, D4, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({F3, D4, CAPTURE, BLACK_KNIGHT, NOTYPE});
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_KNIGHT), 0x8040000);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_KNIGHT), 0x4000000000000000);
}

TEST_F(PositionTest, ArbitraryGame) {
    startingPosition.makeMove({E2, E4, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({C7, C5, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({D2, D4, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({D8, B6, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({G1, F3, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({C5, D4, CAPTURE, WHITE_PAWN, NOTYPE});
    startingPosition.makeMove({F3, D4, CAPTURE, BLACK_PAWN, NOTYPE});
    startingPosition.makeMove({E7, E5, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({D4, F3, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({B8, C6, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({F1, D3, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({D7, D5, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({E4, D5, CAPTURE, BLACK_PAWN, NOTYPE});
    startingPosition.makeMove({G8, F6, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({D5, C6, CAPTURE, BLACK_KNIGHT, NOTYPE});
    startingPosition.makeMove({E5, E4, NORMAL, NOPIECE, NOTYPE});
    
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_PAWN), 0x4000000E700);
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_KNIGHT), 0x200002);
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_BISHOP), 0x80004);
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_ROOK), 0x81);
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_QUEEN), 0x8);
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_KING), 0x10);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_PAWN), 0xE3000010000000);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_KNIGHT), 0x200000000000);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_BISHOP), 0x2400000000000000);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_ROOK), 0x8100000000000000);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_QUEEN), 0x20000000000);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_KING), 0x1000000000000000);

    EXPECT_TRUE(startingPosition.whiteCanKingsideCastle());
    EXPECT_TRUE(startingPosition.whiteCanQueensideCastle());
    EXPECT_TRUE(startingPosition.blackCanKingsideCastle());
    EXPECT_TRUE(startingPosition.blackCanQueensideCastle());

    EXPECT_EQ(startingPosition.getEnPassantTarget(), NOSQUARE);

    EXPECT_EQ(startingPosition.getHalfmoveClock(), 0);
    EXPECT_EQ(startingPosition.getFullmoveCounter(), 9);
}

TEST_F(PositionTest, MovingWhiteKingsRookLosesKingsideCastlingRights){
    Position position = Position();
    position.parseFen("r1bqkbnr/ppp2ppp/2np4/4p3/4P3/3B1N2/PPPP1PPP/RNBQK2R w KQkq - 2 4");
    EXPECT_TRUE(position.whiteCanKingsideCastle());
    EXPECT_TRUE(position.whiteCanQueensideCastle());
    EXPECT_TRUE(position.blackCanKingsideCastle());
    EXPECT_TRUE(position.blackCanQueensideCastle());
    position.makeMove({H1, G1, NORMAL, NOPIECE, NOTYPE});
    EXPECT_FALSE(position.whiteCanKingsideCastle());
    EXPECT_TRUE(position.whiteCanQueensideCastle());
    EXPECT_TRUE(position.blackCanKingsideCastle());
    EXPECT_TRUE(position.blackCanQueensideCastle());
}

TEST_F(PositionTest, MovingWhiteQueensRookLosesQueensideCastlingRights) {
    Position position = Position();
    position.parseFen("r1bqkb1r/ppp2ppp/2n2n2/3pp1B1/3P4/2NQ4/PPP1PPPP/R3KBNR w KQkq e6 0 5");
    EXPECT_TRUE(position.whiteCanKingsideCastle());
    EXPECT_TRUE(position.whiteCanQueensideCastle());
    EXPECT_TRUE(position.blackCanKingsideCastle());
    EXPECT_TRUE(position.blackCanQueensideCastle());
    position.makeMove({A1, B1, NORMAL, NOPIECE, NOTYPE});
    EXPECT_TRUE(position.whiteCanKingsideCastle());
    EXPECT_FALSE(position.whiteCanQueensideCastle());
    EXPECT_TRUE(position.blackCanKingsideCastle());
    EXPECT_TRUE(position.blackCanQueensideCastle());
}

TEST_F(PositionTest, DoublePushingPawnSetsEnPassantTarget) {
    for (int i = 0; i < 8; i++){
        U8 from = A2 + i;
        U8 to = A4 + i;
        startingPosition.makeMove({from, to, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
        EXPECT_EQ(startingPosition.getEnPassantTarget(), i+16);
        from = A7 + i;
        to = A5 + i;
        startingPosition.makeMove({from, to, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
        EXPECT_EQ(startingPosition.getEnPassantTarget(), i+40);
    }
}

TEST_F(PositionTest, WhiteEnPassantLeftToRightCapture) {
    startingPosition.makeMove({E2, E4, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({D7, D5, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({E4, D5, CAPTURE, BLACK_PAWN, NOTYPE});
    startingPosition.makeMove({E7, E5, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({D5, E6, EN_PASSANT, BLACK_PAWN, NOTYPE});
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_PAWN), 0x10000000EF00);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_PAWN), 0xE7000000000000);
}

TEST_F(PositionTest, WhiteEnPassantRightToLeftCapture) {
    startingPosition.makeMove({E2, E4, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({D7, D5, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({E4, D5, CAPTURE, BLACK_PAWN, NOTYPE});
    startingPosition.makeMove({C7, C5, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({D5, C6, EN_PASSANT, BLACK_PAWN, NOTYPE});
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_PAWN), 0x4000000EF00);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_PAWN), 0xF3000000000000);
}

TEST_F(PositionTest, BlackEnPassantLeftToRightCapture) {
    startingPosition.makeMove({E2, E4, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({D7, D5, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({B1, C3, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({D5, E4, CAPTURE, WHITE_PAWN, NOTYPE});
    startingPosition.makeMove({F2, F4, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({E4, F3, EN_PASSANT, WHITE_PAWN, NOTYPE});
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_PAWN), 0xCF00);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_PAWN), 0xF7000000200000);
}

TEST_F(PositionTest, BlackEnPassantRightToLeftCapture) {
    startingPosition.makeMove({E2, E4, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({D7, D5, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({B1, C3, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({D5, E4, CAPTURE, WHITE_PAWN, NOTYPE});
    startingPosition.makeMove({D2, D4, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({E4, D3, EN_PASSANT, WHITE_PAWN, NOTYPE});
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_PAWN), 0xE700);
    EXPECT_EQ(startingPosition.getOccupiedSquares(BLACK_PAWN), 0xF7000000080000);
}

TEST_F(PositionTest, WhiteKingsideCastle) {
    startingPosition.makeMove({E2, E4, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({D7, D5, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE});
    startingPosition.makeMove({G1, F3, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({B8, C6, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({F1, C4, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({G8, F6, NORMAL, NOPIECE, NOTYPE});
    startingPosition.makeMove({E1, G1, KINGSIDE_CASTLE, NOPIECE, NOTYPE});
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_KING), 0x40);
    EXPECT_EQ(startingPosition.getOccupiedSquares(WHITE_ROOK), 0x21);
}

TEST_F(PositionTest, WhiteQueensideCastle) {
    Position position = Position();
    position.parseFen("r1b2q1k/p1p1b2p/p7/3pP3/3B1r2/1N3P2/PPPQ1P1P/R3K2R w KQ - 3 18");
    position.makeMove({E1, C1, QUEENSIDE_CASTLE, NOPIECE, NOTYPE});
    EXPECT_EQ(position.getOccupiedSquares(WHITE_KING), 0x4);
    EXPECT_EQ(position.getOccupiedSquares(WHITE_ROOK), 0x88);
}

TEST_F(PositionTest, BlackKingsideCastle) {
    Position position = Position();
    position.parseFen("r1bqk2r/ppp1bppp/n7/3pP3/4NB2/5N2/PPP1PPPP/R2QKB1R b KQkq - 0 8");
    position.makeMove({E8, G8, KINGSIDE_CASTLE, NOPIECE, NOTYPE});
    EXPECT_EQ(position.getOccupiedSquares(BLACK_KING), 0x4000000000000000);
    EXPECT_EQ(position.getOccupiedSquares(BLACK_ROOK), 0x2100000000000000);
}

TEST_F(PositionTest, MovingWhiteKingLosesAllCastlingRight) {
    Position position = Position();
    position.parseFen("r3k2r/pppq1ppp/2nbbn2/3pp3/3PP3/2NBBN2/PPPQ1PPP/R3K2R w KQkq - 10 8");
    EXPECT_TRUE(position.whiteCanKingsideCastle());
    EXPECT_TRUE(position.whiteCanQueensideCastle());
    EXPECT_TRUE(position.blackCanKingsideCastle());
    EXPECT_TRUE(position.blackCanQueensideCastle());
    position.makeMove({E1, E2, NORMAL, NOPIECE, NOTYPE});
    EXPECT_FALSE(position.whiteCanKingsideCastle());
    EXPECT_FALSE(position.whiteCanQueensideCastle());
    EXPECT_TRUE(position.blackCanKingsideCastle());
    EXPECT_TRUE(position.blackCanQueensideCastle());
}

TEST_F(PositionTest, MovingBlackKingsRookLosesKingsideCastlingRights){
    Position position = Position();
    position.parseFen("r3k2r/pppq1ppp/2nbbn2/3Pp3/3P4/2NBBN2/PPPQ1PPP/R3K2R b KQkq - 0 8");
    EXPECT_TRUE(position.whiteCanKingsideCastle());
    EXPECT_TRUE(position.whiteCanQueensideCastle());
    EXPECT_TRUE(position.blackCanKingsideCastle());
    EXPECT_TRUE(position.blackCanQueensideCastle());
    position.makeMove({H8, G8, NORMAL, NOPIECE, NOTYPE});
    EXPECT_TRUE(position.whiteCanKingsideCastle());
    EXPECT_TRUE(position.whiteCanQueensideCastle());
    EXPECT_FALSE(position.blackCanKingsideCastle());
    EXPECT_TRUE(position.blackCanQueensideCastle());
}

TEST_F(PositionTest, MovingBlackQueensRookLosesQueensideCastlingRights) {
    Position position = Position();
    position.parseFen("r3k2r/pppq1ppp/2nbbn2/3Pp3/3P4/2NBBN2/PPPQ1PPP/R3K2R b KQkq - 0 8");
    EXPECT_TRUE(position.whiteCanKingsideCastle());
    EXPECT_TRUE(position.whiteCanQueensideCastle());
    EXPECT_TRUE(position.blackCanKingsideCastle());
    EXPECT_TRUE(position.blackCanQueensideCastle());
    position.makeMove({A8, B8, NORMAL, NOPIECE, NOTYPE});
    EXPECT_TRUE(position.whiteCanKingsideCastle());
    EXPECT_TRUE(position.whiteCanQueensideCastle());
    EXPECT_TRUE(position.blackCanKingsideCastle());
    EXPECT_FALSE(position.blackCanQueensideCastle());
}

TEST_F(PositionTest, MovingBlackKingLosesAllCastlingRight) {
    Position position = Position();
    position.parseFen("r3k2r/pppq1ppp/2nbbn2/3Pp3/3P4/2NBBN2/PPPQ1PPP/R3K2R b KQkq - 0 8");
    EXPECT_TRUE(position.whiteCanKingsideCastle());
    EXPECT_TRUE(position.whiteCanQueensideCastle());
    EXPECT_TRUE(position.blackCanKingsideCastle());
    EXPECT_TRUE(position.blackCanQueensideCastle());
    
    position.makeMove({E8, E7, NORMAL, NOPIECE, NOTYPE});
    EXPECT_TRUE(position.whiteCanKingsideCastle());
    EXPECT_TRUE(position.whiteCanQueensideCastle());
    EXPECT_FALSE(position.blackCanKingsideCastle());
    EXPECT_FALSE(position.blackCanQueensideCastle());
}