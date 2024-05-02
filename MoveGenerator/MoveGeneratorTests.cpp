#include "gtest/gtest.h"
#include "MoveGenerator.h"

class MoveGeneratorTest : public ::testing::Test {
protected:
    Position startingPosition = Position();
    Position e4BlackToMovePosition = Position();
    Position arbitraryWhiteToMovePosition = Position();
    Position arbitraryBlackToMovePosition = Position();
    MoveGenerator moveGenerator = MoveGenerator();

    void SetUp() override {
        startingPosition.parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        e4BlackToMovePosition.parseFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
        arbitraryWhiteToMovePosition.parseFen("r1bqk2r/ppp4p/n2p1ppn/2bPp3/1PB1P3/2P2N1P/P4PP1/RNBQ1RK1 w kq - 0 10");
        arbitraryBlackToMovePosition.parseFen("2kr3r/pppq1p2/2n1bn1p/3pp1p1/B3P3/1P1P1N1P/P1PB1PP1/R2Q1RK1 b - - 0 13");
    }

    void assertEqualMove(Move move, U8 from, U8 to, MoveType type, Piece captured, PieceType promotion) {
        EXPECT_EQ(move.from, from);
        EXPECT_EQ(move.to, to);
        EXPECT_EQ(move.type, type);
        EXPECT_EQ(move.captured, captured);
        EXPECT_EQ(move.promotion, promotion);
    };
};

TEST_F(MoveGeneratorTest, startingPositionAmoutOfMoves) {
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(startingPosition), 20);
}

TEST_F(MoveGeneratorTest, startingPositionPawnMoves) {
    moveGenerator.generatePseudoLegalMoves(startingPosition);
    for (int i = 0; i < 8; i++) {
        Move move = moveGenerator.pseudoLegalMoves[i];
        assertEqualMove(move, i+8, i+16, NORMAL, NOPIECE, NOTYPE);
    }
    for (int i = 8; i < 16; i++) {
        Move move = moveGenerator.pseudoLegalMoves[i];
        assertEqualMove(move, i, i+16, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE);
    }
}

TEST_F(MoveGeneratorTest, startingPositionKnightMoves) {
    moveGenerator.generatePseudoLegalMoves(startingPosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[16], B1, A3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[17], B1, C3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[18], G1, F3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[19], G1, H3, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, e4BlackToMoveAmountOfMoves) {
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(e4BlackToMovePosition), 20);
}

TEST_F(MoveGeneratorTest, e4BlackToMovePawnMoves) {
    moveGenerator.generatePseudoLegalMoves(e4BlackToMovePosition);
    for (int i = 0; i < 8; i++) {
        Move move = moveGenerator.pseudoLegalMoves[i];
        assertEqualMove(move, i+48, i+40, NORMAL, NOPIECE, NOTYPE);
    }
    for (int i = 8; i < 16; i++) {
        Move move = moveGenerator.pseudoLegalMoves[i];
        assertEqualMove(move, i+40, i+24, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE);
    }
}

TEST_F(MoveGeneratorTest, e4BlackToMoveKnightMoves) {
    moveGenerator.generatePseudoLegalMoves(e4BlackToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[16], B8, A6, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[17], B8, C6, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[18], G8, F6, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[19], G8, H6, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveAmountOfMoves) {
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(arbitraryWhiteToMovePosition), 39);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMovePawnMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[0], A2, A3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[1], G2, G3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[2], H3, H4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[3], B4, B5, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[4], A2, A4, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[5], G2, G4, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[6], B4, C5, CAPTURE, BLACK_BISHOP, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitrariWhiteToMoveKnightMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[7], B1, D2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[8], B1, A3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[9], F3, E1, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[10], F3, D2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[11], F3, H2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[12], F3, D4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[13], F3, H4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[14], F3, E5, CAPTURE, BLACK_PAWN, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[15], F3, G5, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveKingMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[16], G1, H1, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[17], G1, H2, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveBishopMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[18], C1, B2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[19], C1, D2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[20], C1, A3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[21], C1, E3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[22], C1, F4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[23], C1, G5, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[24], C1, H6, CAPTURE, BLACK_KNIGHT, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[25], C4, E2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[26], C4, B3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[27], C4, D3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[28], C4, B5, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[29], C4, A6, CAPTURE, BLACK_KNIGHT, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveRookMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[30], F1, E1, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryWhiteToMoveQueenMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryWhiteToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[31], D1, E1, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[32], D1, C2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[33], D1, D2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[34], D1, E2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[35], D1, B3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[36], D1, D3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[37], D1, A4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[38], D1, D4, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveAmountOfMoves) {
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(arbitraryBlackToMovePosition), 33);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMovePawnMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[0], D5, D4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[1], G5, G4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[2], H6, H5, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[3], A7, A6, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[4], B7, B6, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[5], A7, A5, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[6], B7, B5, DOUBLE_PAWN_PUSH, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[7], D5, E4, CAPTURE, WHITE_PAWN, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveKnightMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[8], C6, B4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[9], C6, D4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[10], C6, A5, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[11], C6, E7, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[12], C6, B8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[13], F6, E4, CAPTURE, WHITE_PAWN, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[14], F6, G4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[15], F6, H5, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[16], F6, H7, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[17], F6, E8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[18], F6, G8, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveKingMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[19], C8, B8, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveBishopMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[20], E6, H3, CAPTURE, WHITE_PAWN, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[21], E6, G4, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[22], E6, F5, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveRookMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[23], D8, E8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[24], D8, F8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[25], D8, G8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[26], H8, H7, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[27], H8, E8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[28], H8, F8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[29], H8, G8, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, arbitraryBlackToMoveQueenMoves) {
    moveGenerator.generatePseudoLegalMoves(arbitraryBlackToMovePosition);
    assertEqualMove(moveGenerator.pseudoLegalMoves[30], D7, D6, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[31], D7, E7, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[32], D7, E8, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteEnPassantLeftToRight) {
    Position whiteEnPassantLeftToRightPosition = Position();
    whiteEnPassantLeftToRightPosition.parseFen("r1b1kb1r/2qn3p/p1n5/1pppPppP/3p4/NBP2N2/PP3PP1/R1BQ1RK1 w kq f6 0 15");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(whiteEnPassantLeftToRightPosition), 35);
    assertEqualMove(moveGenerator.pseudoLegalMoves[6], E5, F6, EN_PASSANT, BLACK_PAWN, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteEnPassantRighToLeft) {
    Position whiteEnPassantRightToLeftPosition = Position();
    whiteEnPassantRightToLeftPosition.parseFen("r1b1kb1r/2qn1p1p/p1np4/1pp3pP/3pP3/N1P2N2/PPB2PP1/R1BQ1RK1 w kq g6 0 13");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(whiteEnPassantRightToLeftPosition), 36);
    assertEqualMove(moveGenerator.pseudoLegalMoves[8], H5, G6, EN_PASSANT, BLACK_PAWN, NOTYPE);
}


TEST_F(MoveGeneratorTest, blackEnPassantLeftToRight) {
    Position blackEnPassantLeftToRightPosition = Position();
    blackEnPassantLeftToRightPosition.parseFen("r1b1kb1r/pp3ppp/1qnp1n2/1B2p3/2pPP3/N1P2N1P/PP3PP1/R1BQ1RK1 b kq d3 0 8");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(blackEnPassantLeftToRightPosition), 39);
    assertEqualMove(moveGenerator.pseudoLegalMoves[7], C4, D3, EN_PASSANT, WHITE_PAWN, NOTYPE);
}

TEST_F(MoveGeneratorTest, blackEnPassantRightToLeft) {
    Position blackEnPassantRightToLeftPosition = Position();
    blackEnPassantRightToLeftPosition.parseFen("r1b1kb1r/pp3ppp/1qnp1n2/1B2p3/1Pp1P3/N1P2N1P/P2P1PP1/R1BQ1RK1 b kq b3 0 8");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(blackEnPassantRightToLeftPosition), 40);
    assertEqualMove(moveGenerator.pseudoLegalMoves[7], C4, B3, EN_PASSANT, WHITE_PAWN, NOTYPE);
}

TEST_F(MoveGeneratorTest, whitePawnPromotion) {
    Position whitePromotePosition = Position();
    whitePromotePosition.parseFen("r7/1kn2P2/1q6/8/2p5/4B3/6K1/5R2 w - - 0 1");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(whitePromotePosition), 33);
    assertEqualMove(moveGenerator.pseudoLegalMoves[0], F7, F8, PROMOTION, NOPIECE, QUEEN);
    assertEqualMove(moveGenerator.pseudoLegalMoves[1], F7, F8, PROMOTION, NOPIECE, ROOK);
    assertEqualMove(moveGenerator.pseudoLegalMoves[2], F7, F8, PROMOTION, NOPIECE, BISHOP);
    assertEqualMove(moveGenerator.pseudoLegalMoves[3], F7, F8, PROMOTION, NOPIECE, KNIGHT);
}

TEST_F(MoveGeneratorTest, whitePawnCapturePromotion) {
    Position whiteCapturePromotePosition = Position();
    whiteCapturePromotePosition.parseFen("r3r1b1/1kn2P2/nq6/5Q2/2p5/4B3/6K1/5R2 w - - 0 1");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(whiteCapturePromotePosition), 61);
    assertEqualMove(moveGenerator.pseudoLegalMoves[0], F7, F8, PROMOTION, NOPIECE, QUEEN);
    assertEqualMove(moveGenerator.pseudoLegalMoves[1], F7, F8, PROMOTION, NOPIECE, ROOK);
    assertEqualMove(moveGenerator.pseudoLegalMoves[2], F7, F8, PROMOTION, NOPIECE, BISHOP);
    assertEqualMove(moveGenerator.pseudoLegalMoves[3], F7, F8, PROMOTION, NOPIECE, KNIGHT);

    assertEqualMove(moveGenerator.pseudoLegalMoves[4], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, QUEEN);
    assertEqualMove(moveGenerator.pseudoLegalMoves[5], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, ROOK);
    assertEqualMove(moveGenerator.pseudoLegalMoves[6], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, BISHOP);
    assertEqualMove(moveGenerator.pseudoLegalMoves[7], F7, E8, PROMOTION_CAPTURE, BLACK_ROOK, KNIGHT);

    assertEqualMove(moveGenerator.pseudoLegalMoves[8], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, QUEEN);
    assertEqualMove(moveGenerator.pseudoLegalMoves[9], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, ROOK);
    assertEqualMove(moveGenerator.pseudoLegalMoves[10], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, BISHOP);
    assertEqualMove(moveGenerator.pseudoLegalMoves[11], F7, G8, PROMOTION_CAPTURE, BLACK_BISHOP, KNIGHT);
};

TEST_F(MoveGeneratorTest, blackPawnPromotion) {
    Position blackToPromotePosition = Position();
    blackToPromotePosition.parseFen("8/1k6/8/8/8/8/1p4K1/8 b - - 0 1");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(blackToPromotePosition), 12);
    assertEqualMove(moveGenerator.pseudoLegalMoves[0], B2, B1, PROMOTION, NOPIECE, QUEEN);
    assertEqualMove(moveGenerator.pseudoLegalMoves[1], B2, B1, PROMOTION, NOPIECE, ROOK);
    assertEqualMove(moveGenerator.pseudoLegalMoves[2], B2, B1, PROMOTION, NOPIECE, BISHOP);
    assertEqualMove(moveGenerator.pseudoLegalMoves[3], B2, B1, PROMOTION, NOPIECE, KNIGHT);
}

TEST_F(MoveGeneratorTest, blackPawnCapturePromotion) {
    Position blackToCapturePromotePosition = Position();
    blackToCapturePromotePosition.parseFen("8/1k6/8/8/8/8/1p4K1/R1B5 b - - 0 1");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(blackToCapturePromotePosition), 20);
    assertEqualMove(moveGenerator.pseudoLegalMoves[0], B2, B1, PROMOTION, NOPIECE, QUEEN);
    assertEqualMove(moveGenerator.pseudoLegalMoves[1], B2, B1, PROMOTION, NOPIECE, ROOK);
    assertEqualMove(moveGenerator.pseudoLegalMoves[2], B2, B1, PROMOTION, NOPIECE, BISHOP);
    assertEqualMove(moveGenerator.pseudoLegalMoves[3], B2, B1, PROMOTION, NOPIECE, KNIGHT);

    assertEqualMove(moveGenerator.pseudoLegalMoves[4], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, QUEEN);
    assertEqualMove(moveGenerator.pseudoLegalMoves[5], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, ROOK);
    assertEqualMove(moveGenerator.pseudoLegalMoves[6], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, BISHOP);
    assertEqualMove(moveGenerator.pseudoLegalMoves[7], B2, A1, PROMOTION_CAPTURE, WHITE_ROOK, KNIGHT);

    assertEqualMove(moveGenerator.pseudoLegalMoves[8], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, QUEEN);
    assertEqualMove(moveGenerator.pseudoLegalMoves[9], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, ROOK);
    assertEqualMove(moveGenerator.pseudoLegalMoves[10], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, BISHOP);
    assertEqualMove(moveGenerator.pseudoLegalMoves[11], B2, C1, PROMOTION_CAPTURE, WHITE_BISHOP, KNIGHT);
}

TEST_F(MoveGeneratorTest, whiteKingMovesAndCaptures) {
    Position whiteKingCapturePosition = Position();
    whiteKingCapturePosition.parseFen("8/1n6/1k6/8/4bp2/5K2/4NP2/8 w - - 0 1");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(whiteKingCapturePosition), 12);
    assertEqualMove(moveGenerator.pseudoLegalMoves[6], F3, G2, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[7], F3, E3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[8], F3, G3, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[9], F3, E4, CAPTURE, BLACK_BISHOP, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[10], F3, F4, CAPTURE, BLACK_PAWN, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[11], F3, G4, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, blackKingMovesAndCaptures) {
    Position blackKingCapturePosition = Position();
    blackKingCapturePosition.parseFen("4r3/4kp2/3PnB2/8/2Q5/1K6/8/8 b - - 0 1");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(blackKingCapturePosition), 20);
    assertEqualMove(moveGenerator.pseudoLegalMoves[8], E7, D6, CAPTURE, WHITE_PAWN, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[9], E7, F6, CAPTURE, WHITE_BISHOP, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[10], E7, D7, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[11], E7, D8, NORMAL, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[12], E7, F8, NORMAL, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteKingSideCastle) {
    Position whiteKingSideCastle = Position();
    whiteKingSideCastle.parseFen("r1bqk1nr/pppp2pp/n4p2/2bPp3/2B1P3/2P2N2/PP3PPP/RNBQK2R w KQkq - 3 7");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(whiteKingSideCastle), 41);
    assertEqualMove(moveGenerator.pseudoLegalMoves[40], E1, G1, KINGSIDE_CASTLE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteQueenSideCastle) {
    Position whiteQueenSideCastle = Position();
    whiteQueenSideCastle.parseFen("rnbk1b1Q/ppppqp1p/5np1/8/4P3/2NPB3/PPP2PPP/R3KBNR w KQ - 5 8");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(whiteQueenSideCastle), 43);
    assertEqualMove(moveGenerator.pseudoLegalMoves[42], E1, C1, QUEENSIDE_CASTLE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteCastleBothWays) {
    Position whiteCastleBothWays = Position();
    whiteCastleBothWays.parseFen("2bqkb1r/rpp2ppp/p1np4/4p2n/3PP3/2NBBN2/PPP1QPPP/R3K2R w KQk - 4 8");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(whiteCastleBothWays), 41);
    assertEqualMove(moveGenerator.pseudoLegalMoves[39], E1, G1, KINGSIDE_CASTLE, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[40], E1, C1, QUEENSIDE_CASTLE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, whiteKingSideCastleUnderAttack) {
    Position whiteCantCastle = Position();
    whiteCantCastle.parseFen("rnbqk2r/pppp1p1p/1b3np1/4p3/4P2N/3B1P2/PPPP2PP/RNBQK2R w KQkq - 1 6");
     EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(whiteCantCastle), 25);

}

TEST_F(MoveGeneratorTest, blackKingSideCastle) {
    Position blackKingSideCastle = Position();
    blackKingSideCastle.parseFen("rnbqk2r/pppp1ppp/3b1n2/4p3/2B5/4PQ2/PPPPNPPP/RNB1K2R b KQkq - 5 4");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(blackKingSideCastle), 29);
    assertEqualMove(moveGenerator.pseudoLegalMoves[28], E8, G8, KINGSIDE_CASTLE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, blackQueenSideCastle) {
    Position blackQueenSideCastle = Position();
    blackQueenSideCastle.parseFen("r3k1r1/p1ppqppp/bpnb1n2/4p3/2B5/2NPP1Q1/PPP2PPP/RNBK2R1 b q - 3 9");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(blackQueenSideCastle), 33);
    assertEqualMove(moveGenerator.pseudoLegalMoves[32], E8, C8, QUEENSIDE_CASTLE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, blackCastleBothWays) {
    Position blackCastleBothWays = Position();
    blackCastleBothWays.parseFen("r3k2r/ppp1pp1p/2q1bnpb/3p4/3P4/1PP1P1PP/P3BP2/RNB1K1NR b KQkq - 0 9");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(blackCastleBothWays), 39);
    // 5+5+10+5+9+3+2
    assertEqualMove(moveGenerator.pseudoLegalMoves[37], E8, G8, KINGSIDE_CASTLE, NOPIECE, NOTYPE);
    assertEqualMove(moveGenerator.pseudoLegalMoves[38], E8, C8, QUEENSIDE_CASTLE, NOPIECE, NOTYPE);
}

TEST_F(MoveGeneratorTest, blackQueenSideCastleUnderAttack) {
    Position blackCantCastle = Position();
    blackCantCastle.parseFen("r3k1r1/p1pp1ppp/Bpnbqn2/4p3/8/2NPP1Q1/PPP2PPP/RNBK2R1 b q - 0 10");
    EXPECT_EQ(moveGenerator.generatePseudoLegalMoves(blackCantCastle), 37);
}