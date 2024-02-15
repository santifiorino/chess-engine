#include "gtest/gtest.h"
#include "Position.h"

class PositionTest : public ::testing::Test {
protected:
    Position startingPosition = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

};

TEST_F(PositionTest, StartingPositionPiecesSquareCentric) {
    EXPECT_EQ(startingPosition.getPieceAt(0), WHITE_ROOK) << "Expected WHITE_ROOK at index 0";
    EXPECT_EQ(startingPosition.getPieceAt(1), WHITE_KNIGHT) << "Expected WHITE_KNIGHT at index 1";
    EXPECT_EQ(startingPosition.getPieceAt(2), WHITE_BISHOP) << "Expected WHITE_BISHOP at index 2";
    EXPECT_EQ(startingPosition.getPieceAt(3), WHITE_QUEEN) << "Expected WHITE_QUEEN at index 3";
    EXPECT_EQ(startingPosition.getPieceAt(4), WHITE_KING) << "Expected WHITE_KING at index 4";
    EXPECT_EQ(startingPosition.getPieceAt(5), WHITE_BISHOP) << "Expected WHITE_BISHOP at index 5";
    EXPECT_EQ(startingPosition.getPieceAt(6), WHITE_KNIGHT) << "Expected WHITE_KNIGHT at index 6";
    EXPECT_EQ(startingPosition.getPieceAt(7), WHITE_ROOK) << "Expected WHITE_ROOK at index 7";
    for (int i = 8; i < 16; i++) {
        EXPECT_EQ(startingPosition.getPieceAt(i), WHITE_PAWN) << "Expected WHITE_PAWN at index " << i;
    }

    for (int i = 16; i < 48; i++) {
        EXPECT_EQ(startingPosition.getPieceAt(i), EMPTY) << "Expected EMPTY at index " << i;
    }

    for (int i = 48; i < 56; i++) {
        EXPECT_EQ(startingPosition.getPieceAt(i), BLACK_PAWN) << "Expected BLACK_PAWN at index " << i;
    }
    EXPECT_EQ(startingPosition.getPieceAt(56), BLACK_ROOK) << "Expected BLACK_ROOK at index 56";
    EXPECT_EQ(startingPosition.getPieceAt(57), BLACK_KNIGHT) << "Expected BLACK_KNIGHT at index 57";
    EXPECT_EQ(startingPosition.getPieceAt(58), BLACK_BISHOP) << "Expected BLACK_BISHOP at index 58";
    EXPECT_EQ(startingPosition.getPieceAt(59), BLACK_QUEEN) << "Expected BLACK_QUEEN at index 59";
    EXPECT_EQ(startingPosition.getPieceAt(60), BLACK_KING) << "Expected BLACK_KING at index 60";
    EXPECT_EQ(startingPosition.getPieceAt(61), BLACK_BISHOP) << "Expected BLACK_BISHOP at index 61";
    EXPECT_EQ(startingPosition.getPieceAt(62), BLACK_KNIGHT) << "Expected BLACK_KNIGHT at index 62";
    EXPECT_EQ(startingPosition.getPieceAt(63), BLACK_ROOK) << "Expected BLACK_ROOK at index 63";
}

TEST_F(PositionTest, StartingPositionBitBoard) {
    // 1111 1111 0000 0000
    EXPECT_EQ(startingPosition.getBitboard(WHITE_PAWN), 0xFF00) << "Expected WHITE_PAWN bitboard to be 0xFF00";
    // 0100 0010
    EXPECT_EQ(startingPosition.getBitboard(WHITE_KNIGHT), 0x42) << "Expected WHITE_KNIGHT bitboard to be 0x42";
    // 0010 0100
    EXPECT_EQ(startingPosition.getBitboard(WHITE_BISHOP), 0x24) << "Expected WHITE_BISHOP bitboard to be 0x24";
    // 1000 0001
    EXPECT_EQ(startingPosition.getBitboard(WHITE_ROOK), 0x81) << "Expected WHITE_ROOK bitboard to be 0x81";
    // 0000 1000 (its mirrored)
    EXPECT_EQ(startingPosition.getBitboard(WHITE_QUEEN), 0x8) << "Expected WHITE_QUEEN bitboard to be 0x8";
    // 0001 0000 (its mirrored)
    EXPECT_EQ(startingPosition.getBitboard(WHITE_KING), 0x10) << "Expected WHITE_KING bitboard to be 0x10";
    // 1111 1111 (0000) x 2 x 6
    EXPECT_EQ(startingPosition.getBitboard(BLACK_PAWN), 0xFF000000000000) << "Expected BLACK_PAWN bitboard to be 0xFF000000000000";
    // 0100 0010 (0000) x 2 x 7
    EXPECT_EQ(startingPosition.getBitboard(BLACK_KNIGHT), 0x4200000000000000) << "Expected BLACK_KNIGHT bitboard to be 0x4200000000000000";
    // 0010 0100 (0000) x 2 x 7
    EXPECT_EQ(startingPosition.getBitboard(BLACK_BISHOP), 0x2400000000000000) << "Expected BLACK_BISHOP bitboard to be 0x2400000000000000";
    // 1000 0001 (0000) x 2 x 7
    EXPECT_EQ(startingPosition.getBitboard(BLACK_ROOK), 0x8100000000000000) << "Expected BLACK_ROOK bitboard to be 0x8100000000000000";
    // 0000 1000 (0000) x 2 x 7 (its mirrored)
    EXPECT_EQ(startingPosition.getBitboard(BLACK_QUEEN), 0x800000000000000) << "Expected BLACK_QUEEN bitboard to be 0x800000000000000";
    // 0001 0000 (0000) x 2 x 7 (its mirrored)
    EXPECT_EQ(startingPosition.getBitboard(BLACK_KING), 0x1000000000000000) << "Expected BLACK_KING bitboard to be 0x1000000000000000";
}

TEST_F(PositionTest, StartingPositionEmptySquaresBitboard) {
    // (1111) x 2 x 4 (0000) x 2 x 2
    EXPECT_EQ(startingPosition.getEmptySquares(), 0xFFFFFFFF0000) << "Expected empty squares bitboard to be 0xFFFFFFFF0000";
}

TEST_F(PositionTest, StartingPositionPlayer) {
    EXPECT_EQ(startingPosition.getCurrentPlayer(), WHITE) << "Expected current player to be WHITE";
}

TEST_F(PositionTest, StartingPositionCastlingRights) {
    EXPECT_EQ(startingPosition.whiteCanCastleKingside(), true) << "Expected white to be able to castle kingside";
    EXPECT_EQ(startingPosition.whiteCanCastleQueenside(), true) << "Expected white to be able to castle queenside";
    EXPECT_EQ(startingPosition.blackCanCastleKingside(), true) << "Expected black to be able to castle kingside";
    EXPECT_EQ(startingPosition.blackCanCastleQueenside(), true) << "Expected black to be able to castle queenside";
}

TEST_F(PositionTest, StartingPositionEnPassantTarget) {
    EXPECT_EQ(startingPosition.getEnPassantTarget(), 0) << "Expected no en passant target";
}

TEST_F(PositionTest, EnPassantPositionEnPassantTarget) {
    Position enPassantPosition = Position("rnbqkbnr/1pppp1pp/p7/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 1");
    EXPECT_EQ(enPassantPosition.getEnPassantTarget(), F6) << "Expected en passant target at index 45";
}

TEST_F(PositionTest, StartingPositionHalfmoveClock) {
    EXPECT_EQ(startingPosition.getHalfmoveClock(), 0) << "Expected halfmove clock to be 0";
}

TEST_F(PositionTest, MultipleDigitsHalfmoveClock) {
    Position position = Position("rnbqkb1r/pppp1ppp/5n2/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 18 11");
    EXPECT_EQ(position.getHalfmoveClock(), 18) << "Expected halfmove clock to be 18";
}

TEST_F(PositionTest, StartingPositionFullmoveCounter) {
    EXPECT_EQ(startingPosition.getFullmoveCounter(), 1) << "Expected fullmove counter to be 1";
}

TEST_F(PositionTest, MultipleDigitsFullmoveCounter) {
    Position position = Position("rnbqkb1r/pppp1ppp/5n2/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 18 11");
    EXPECT_EQ(position.getFullmoveCounter(), 11) << "Expected fullmove counter to be 11";
}