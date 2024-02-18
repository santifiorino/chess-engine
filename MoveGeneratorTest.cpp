#include "gtest/gtest.h"
#include "MoveGenerator.h"

class MoveGeneratorTest : public ::testing::Test {
protected:
    Position startingPosition = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Position e4BlackToMovePosition = Position("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    Position arbitraryWhiteToMovePosition = Position("r1bqk2r/ppp4p/n2p1ppn/2bPp3/1PB1P3/2P2N1P/P4PP1/RNBQ1RK1 w kq - 0 10");
    Position arbitraryBlackToMovePosition = Position("2kr3r/pppq1p2/2n1bn1p/3pp1p1/B3P3/1P1P1N1P/P1PB1PP1/R2Q1RK1 b - - 0 13");
    MoveGenerator moveGenerator = MoveGenerator();

    void assertEqualMove(Move move, U8 from, U8 to, MoveType type, Piece captured, PieceType promotion) {
        EXPECT_EQ(move.from, from);
        EXPECT_EQ(move.to, to);
        EXPECT_EQ(move.type, type);
        EXPECT_EQ(move.captured, captured);
        EXPECT_EQ(move.promotion, promotion);
    };
};

TEST_F(MoveGeneratorTest, startingPositionAmoutOfMoves) {
    EXPECT_EQ(moveGenerator.generateMoves(startingPosition), 20);
}

TEST_F(MoveGeneratorTest, startingPositionPawnMoves) {
    moveGenerator.generateMoves(startingPosition);
    for (int i = 0; i < 8; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i+8, i+16, NORMAL, NOPIECE, NOTYPE);
    }
    for (int i = 8; i < 16; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i, i+16, NORMAL, NOPIECE, NOTYPE);
    }
}

TEST_F(MoveGeneratorTest, startingPositionKnightMoves) {
    moveGenerator.generateMoves(startingPosition);
    assertEqualMove(moveGenerator.legalMoves[16], B1, A3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[17], B1, C3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[18], G1, F3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[19], G1, H3, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, e4BlackToMoveAmountOfMoves) {
    EXPECT_EQ(moveGenerator.generateMoves(e4BlackToMovePosition), 20);
}

TEST_F(MoveGeneratorTest, e4BlackToMovePawnMoves) {
    moveGenerator.generateMoves(e4BlackToMovePosition);
    for (int i = 0; i < 8; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i+48, i+40, NORMAL, NOPIECE, NOTYPE);
    }
    for (int i = 8; i < 16; i++) {
        Move move = moveGenerator.legalMoves[i];
        assertEqualMove(move, i+40, i+24, NORMAL, NOPIECE, NOTYPE);
    }
}

TEST_F(MoveGeneratorTest, e4BlackToMoveKnightMoves) {
    moveGenerator.generateMoves(e4BlackToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[16], B8, A6, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[17], B8, C6, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[18], G8, F6, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[19], G8, H6, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveAmountOfMoves) {
    EXPECT_EQ(moveGenerator.generateMoves(arbitraryWhiteToMovePosition), 39);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMovePawnMoves) {
    moveGenerator.generateMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[0], A2, A3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[1], G2, G3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[2], H3, H4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[3], B4, B5, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[4], A2, A4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[5], G2, G4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[6], B4, C5, CAPTURE, BLACK_BISHOP, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitrariWhiteToMoveKnightMoves) {
    moveGenerator.generateMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[7], B1, D2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[8], B1, A3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[9], F3, E1, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[10], F3, D2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[11], F3, H2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[12], F3, D4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[13], F3, H4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[14], F3, E5, CAPTURE, BLACK_PAWN, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[15], F3, G5, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveKingMoves) {
    moveGenerator.generateMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[16], G1, H1, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[17], G1, H2, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveBishopMoves) {
    moveGenerator.generateMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[18], C1, B2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[19], C1, D2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[20], C1, A3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[21], C1, E3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[22], C1, F4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[23], C1, G5, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[24], C1, H6, CAPTURE, BLACK_KNIGHT, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[25], C4, E2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[26], C4, B3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[27], C4, D3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[28], C4, B5, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[29], C4, A6, CAPTURE, BLACK_KNIGHT, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveRookMoves) {
    moveGenerator.generateMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[30], F1, E1, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveQueenMoves) {
    moveGenerator.generateMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[31], D1, E1, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[32], D1, C2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[33], D1, D2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[34], D1, E2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[35], D1, B3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[36], D1, D3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[37], D1, A4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[38], D1, D4, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveAmountOfMoves) {
    EXPECT_EQ(moveGenerator.generateMoves(arbitraryBlackToMovePosition), 33);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMovePawnMoves) {
    moveGenerator.generateMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[0], 35, 27, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[1], 38, 30, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[2], 47, 39, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[3], 48, 40, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[4], 49, 41, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[5], 48, 32, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[6], 49, 33, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[7], 35, 28, CAPTURE, WHITE_PAWN, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveKnightMoves) {
    moveGenerator.generateMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[8], C6, B4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[9], C6, D4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[10], C6, A5, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[11], C6, E7, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[12], C6, B8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[13], F6, E4, CAPTURE, WHITE_PAWN, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[14], F6, G4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[15], F6, H5, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[16], F6, H7, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[17], F6, E8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[18], F6, G8, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveKingMoves) {
    moveGenerator.generateMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[19], C8, B8, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveBishopMoves) {
    moveGenerator.generateMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[20], E6, H3, CAPTURE, WHITE_PAWN, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[21], E6, G4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[22], E6, F5, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveRookMoves) {
    moveGenerator.generateMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[23], D8, E8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[24], D8, F8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[25], D8, G8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[26], H8, H7, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[27], H8, E8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[28], H8, F8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[29], H8, G8, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveQueenMoves) {
    moveGenerator.generateMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.legalMoves[30], D7, D6, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[31], D7, E7, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[32], D7, E8, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteEnPassantLeftToRight) {
    Position whiteEnPassantLeftToRightPosition = Position("r1b1kb1r/2qn3p/p1n5/1pppPppP/3p4/NBP2N2/PP3PP1/R1BQ1RK1 w kq f6 0 15");
    EXPECT_EQ(moveGenerator.generateMoves(whiteEnPassantLeftToRightPosition), 35);
    assertEqualMove(moveGenerator.legalMoves[6], E5, F6, EN_PASSANT, BLACK_PAWN, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteEnPassantRighToLeft) {
    Position whiteEnPassantRightToLeftPosition = Position("r1b1kb1r/2qn1p1p/p1np4/1pp3pP/3pP3/N1P2N2/PPB2PP1/R1BQ1RK1 w kq g6 0 13");
    EXPECT_EQ(moveGenerator.generateMoves(whiteEnPassantRightToLeftPosition), 36);
    assertEqualMove(moveGenerator.legalMoves[8], H5, G6, EN_PASSANT, BLACK_PAWN, NOTYPE);
}


TEST_F(MoveGeneratorTest, blackEnPassantLeftToRight) {
    Position blackEnPassantLeftToRightPosition = Position("r1b1kb1r/pp3ppp/1qnp1n2/1B2p3/2pPP3/N1P2N1P/PP3PP1/R1BQ1RK1 b kq d3 0 8");
    EXPECT_EQ(moveGenerator.generateMoves(blackEnPassantLeftToRightPosition), 39);
    assertEqualMove(moveGenerator.legalMoves[7], C4, D3, EN_PASSANT, WHITE_PAWN, NOTYPE);
}

TEST_F(MoveGeneratorTest, blackEnPassantRightToLeft) {
    Position blackEnPassantRightToLeftPosition = Position("r1b1kb1r/pp3ppp/1qnp1n2/1B2p3/1Pp1P3/N1P2N1P/P2P1PP1/R1BQ1RK1 b kq b3 0 8");
    EXPECT_EQ(moveGenerator.generateMoves(blackEnPassantRightToLeftPosition), 40);
    assertEqualMove(moveGenerator.legalMoves[7], C4, B3, EN_PASSANT, WHITE_PAWN, NOTYPE);
}

TEST_F(MoveGeneratorTest, whitePawnPromotion) {
    Position whitePromotePosition = Position("r7/1kn2P2/1q6/8/2p5/4B3/6K1/5R2 w - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(whitePromotePosition), 33);
    assertEqualMove(moveGenerator.legalMoves[0], F7, F8, PROMOTION, NOPIECE, QUEEN);
    assertEqualMove(moveGenerator.legalMoves[1], F7, F8, PROMOTION, NOPIECE, ROOK);
    assertEqualMove(moveGenerator.legalMoves[2], F7, F8, PROMOTION, NOPIECE, BISHOP);
    assertEqualMove(moveGenerator.legalMoves[3], F7, F8, PROMOTION, NOPIECE, KNIGHT);
}

TEST_F(MoveGeneratorTest, whitePawnCapturePromotion) {
    Position whiteCapturePromotePosition = Position("r3r1b1/1kn2P2/nq6/5Q2/2p5/4B3/6K1/5R2 w - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(whiteCapturePromotePosition), 61);
    assertEqualMove(moveGenerator.legalMoves[0], F7, F8, PROMOTION, NOPIECE, QUEEN);
    assertEqualMove(moveGenerator.legalMoves[1], F7, F8, PROMOTION, NOPIECE, ROOK);
    assertEqualMove(moveGenerator.legalMoves[2], F7, F8, PROMOTION, NOPIECE, BISHOP);
    assertEqualMove(moveGenerator.legalMoves[3], F7, F8, PROMOTION, NOPIECE, KNIGHT);

    assertEqualMove(moveGenerator.legalMoves[4], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, QUEEN);
    assertEqualMove(moveGenerator.legalMoves[5], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, ROOK);
    assertEqualMove(moveGenerator.legalMoves[6], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, BISHOP);
    assertEqualMove(moveGenerator.legalMoves[7], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, KNIGHT);

    assertEqualMove(moveGenerator.legalMoves[8], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, QUEEN);
    assertEqualMove(moveGenerator.legalMoves[9], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, ROOK);
    assertEqualMove(moveGenerator.legalMoves[10], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, BISHOP);
    assertEqualMove(moveGenerator.legalMoves[11], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, KNIGHT);
};

TEST_F(MoveGeneratorTest, blackPawnPromotion) {
    Position blackToPromotePosition = Position("8/1k6/8/8/8/8/1p4K1/8 b - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(blackToPromotePosition), 12);
    assertEqualMove(moveGenerator.legalMoves[0], B2, B1, PROMOTION, NOPIECE, QUEEN);
    assertEqualMove(moveGenerator.legalMoves[1], B2, B1, PROMOTION, NOPIECE, ROOK);
    assertEqualMove(moveGenerator.legalMoves[2], B2, B1, PROMOTION, NOPIECE, BISHOP);
    assertEqualMove(moveGenerator.legalMoves[3], B2, B1, PROMOTION, NOPIECE, KNIGHT);
}

TEST_F(MoveGeneratorTest, blackPawnCapturePromotion) {
    Position blackToCapturePromotePosition = Position("8/1k6/8/8/8/8/1p4K1/R1B5 b - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(blackToCapturePromotePosition), 20);
    assertEqualMove(moveGenerator.legalMoves[0], B2, B1, PROMOTION, NOPIECE, QUEEN);
    assertEqualMove(moveGenerator.legalMoves[1], B2, B1, PROMOTION, NOPIECE, ROOK);
    assertEqualMove(moveGenerator.legalMoves[2], B2, B1, PROMOTION, NOPIECE, BISHOP);
    assertEqualMove(moveGenerator.legalMoves[3], B2, B1, PROMOTION, NOPIECE, KNIGHT);

    assertEqualMove(moveGenerator.legalMoves[4], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, QUEEN);
    assertEqualMove(moveGenerator.legalMoves[5], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, ROOK);
    assertEqualMove(moveGenerator.legalMoves[6], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, BISHOP);
    assertEqualMove(moveGenerator.legalMoves[7], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, KNIGHT);

    assertEqualMove(moveGenerator.legalMoves[8], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, QUEEN);
    assertEqualMove(moveGenerator.legalMoves[9], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, ROOK);
    assertEqualMove(moveGenerator.legalMoves[10], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, BISHOP);
    assertEqualMove(moveGenerator.legalMoves[11], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, KNIGHT);
}

TEST_F(MoveGeneratorTest, whiteKingMovesAndCaptures) {
    Position whiteKingCapturePosition = Position("8/1n6/1k6/8/4bp2/5K2/4NP2/8 w - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(whiteKingCapturePosition), 12);
    assertEqualMove(moveGenerator.legalMoves[6], F3, G2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[7], F3, E3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[8], F3, G3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[9], F3, E4, CAPTURE, BLACK_BISHOP, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[10], F3, F4, CAPTURE, BLACK_PAWN, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[11], F3, G4, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, blackKingMovesAndCaptures) {
    Position blackKingCapturePosition = Position("4r3/4kp2/3PnB2/8/2Q5/1K6/8/8 b - - 0 1");
    EXPECT_EQ(moveGenerator.generateMoves(blackKingCapturePosition), 20);
    assertEqualMove(moveGenerator.legalMoves[8], E7, D6, CAPTURE, WHITE_PAWN, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[9], E7, F6, CAPTURE, WHITE_BISHOP, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[10], E7, D7, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[11], E7, D8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[12], E7, F8, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteKingSideCastle) {
    Position whiteKingSideCastle = Position("r1bqk1nr/pppp2pp/n4p2/2bPp3/2B1P3/2P2N2/PP3PPP/RNBQK2R w KQkq - 3 7");
    EXPECT_EQ(moveGenerator.generateMoves(whiteKingSideCastle), 41);
    assertEqualMove(moveGenerator.legalMoves[40], E1, G1, CASTLE_KINGSIDE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteQueenSideCastle) {
    Position whiteQueenSideCastle = Position("rnbk1b1Q/ppppqp1p/5np1/8/4P3/2NPB3/PPP2PPP/R3KBNR w KQ - 5 8");
    EXPECT_EQ(moveGenerator.generateMoves(whiteQueenSideCastle), 43);
    assertEqualMove(moveGenerator.legalMoves[42], E1, C1, CASTLE_QUEENSIDE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteCastleBothWays) {
    Position whiteCastleBothWays = Position("2bqkb1r/rpp2ppp/p1np4/4p2n/3PP3/2NBBN2/PPP1QPPP/R3K2R w KQk - 4 8");
    EXPECT_EQ(moveGenerator.generateMoves(whiteCastleBothWays), 41);
    assertEqualMove(moveGenerator.legalMoves[39], E1, G1, CASTLE_KINGSIDE, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[40], E1, C1, CASTLE_QUEENSIDE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteKingSideCastleUnderAttack) {
    Position whiteCantCastle = Position("rnbqk2r/pppp1p1p/1b3np1/4p3/4P2N/3B1P2/PPPP2PP/RNBQK2R w KQkq - 1 6");
     EXPECT_EQ(moveGenerator.generateMoves(whiteCantCastle), 25);

}

TEST_F(MoveGeneratorTest, blackKingSideCastle) {
    Position blackKingSideCastle = Position("rnbqk2r/pppp1ppp/3b1n2/4p3/2B5/4PQ2/PPPPNPPP/RNB1K2R b KQkq - 5 4");
    EXPECT_EQ(moveGenerator.generateMoves(blackKingSideCastle), 29);
    assertEqualMove(moveGenerator.legalMoves[28], E8, G8, CASTLE_KINGSIDE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, blackQueenSideCastle) {
    Position blackQueenSideCastle = Position("r3k1r1/p1ppqppp/bpnb1n2/4p3/2B5/2NPP1Q1/PPP2PPP/RNBK2R1 b q - 3 9");
    EXPECT_EQ(moveGenerator.generateMoves(blackQueenSideCastle), 33);
    assertEqualMove(moveGenerator.legalMoves[32], E8, C8, CASTLE_QUEENSIDE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, blackCastleBothWays) {
    Position blackCastleBothWays = Position("r3k2r/ppp1pp1p/2q1bnpb/3p4/3P4/1PP1P1PP/P3BP2/RNB1K1NR b KQkq - 0 9");
    EXPECT_EQ(moveGenerator.generateMoves(blackCastleBothWays), 39);
    // 5+5+10+5+9+3+2
    assertEqualMove(moveGenerator.legalMoves[37], E8, G8, CASTLE_KINGSIDE, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.legalMoves[38], E8, C8, CASTLE_QUEENSIDE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, blackQueenSideCastleUnderAttack) {
    Position blackCantCastle = Position("r3k1r1/p1pp1ppp/Bpnbqn2/4p3/8/2NPP1Q1/PPP2PPP/RNBK2R1 b q - 0 10");
    EXPECT_EQ(moveGenerator.generateMoves(blackCantCastle), 37);
}