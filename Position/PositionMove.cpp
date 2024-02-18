#ifndef POSITIONMOVE_CPP
#define POSITIONMOVE_CPP

#include "Position.h"

void Position::makeMove(Move move){
    Piece movedPiece = getPieceAt(move.from);
    U64 fromSquare = 1ULL << move.from;
    U64 toSquare = 1ULL << move.to;

    if (move.type == CAPTURE || move.type == PROMOTION_CAPTURE) {
        // Remove the captured piece from the board
        Piece captured = getPieceAt(move.to);
        bitboards[captured] ^= toSquare;
    } else if (move.type == EN_PASSANT) {
        // Remove the captured pawn from the board
        U8 capturedPawnSquare = move.to + (currentPlayer == WHITE ? -8 : 8);
        bitboards[getPieceAt(capturedPawnSquare)] ^= 1ULL << capturedPawnSquare;
    } else if (move.type == KINGSIDE_CASTLE) {
        // Move the rook
        if (currentPlayer == WHITE) {
            removeCastlingAbility(WHITE, NORMAL);
            bitboards[WHITE_ROOK] ^= 1ULL << H1;
            bitboards[WHITE_ROOK] |= 1ULL << F1;
        } else {
            removeCastlingAbility(BLACK, NORMAL);
            bitboards[BLACK_ROOK] ^= 1ULL << H8;
            bitboards[BLACK_ROOK] |= 1ULL << F8;
        }
    } else if (move.type == QUEENSIDE_CASTLE) {
        // Move the rook
        if (currentPlayer == WHITE) {
            removeCastlingAbility(WHITE, NORMAL);
            bitboards[WHITE_ROOK] ^= 1ULL << A1;
            bitboards[WHITE_ROOK] |= 1ULL << D1;
        } else {
            removeCastlingAbility(BLACK, NORMAL);
            bitboards[BLACK_ROOK] ^= 1ULL << A8;
            bitboards[BLACK_ROOK] |= 1ULL << D8;
        }
    }
    // Remove the piece from the original square
    bitboards[movedPiece] ^= fromSquare;
    // Move the piece to the new square
    if (move.type != PROMOTION && move.type != PROMOTION_CAPTURE) {
        bitboards[movedPiece] ^= toSquare;
    } else {
        // Instead of placing the same piece, place the promoted piece
        bitboards[move.promotion + 6 * currentPlayer] ^= toSquare;
    }

    // Update current player
    currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;

    // Update castling ability
    if (movedPiece == WHITE_KING) {
        // Remove both castling abilities
        removeCastlingAbility(WHITE, NORMAL);
    } else if (movedPiece == BLACK_KING) {
        removeCastlingAbility(BLACK, NORMAL);
    } else if (movedPiece == WHITE_ROOK) {
        if (move.from == H1) {
            removeCastlingAbility(WHITE, KINGSIDE_CASTLE);
        } else if (move.from == A1) {
            removeCastlingAbility(WHITE, QUEENSIDE_CASTLE);
        }
    } else if (movedPiece == BLACK_ROOK) {
        if (move.from == H8) {
            removeCastlingAbility(BLACK, KINGSIDE_CASTLE);
        } else if (move.from == A8) {
            removeCastlingAbility(BLACK, QUEENSIDE_CASTLE);
        }
    }

    // Update en passant target square
    enPassantTargetSquare = NOSQUARE;
    if (move.type == DOUBLE_PAWN_PUSH) {
        enPassantTargetSquare = (move.from + move.to) / 2;
    }

    // Update halfmove clock
    if (move.type == CAPTURE || movedPiece == WHITE_PAWN || movedPiece == BLACK_PAWN) {
        halfmoveClock = 0;
    } else {
        halfmoveClock++;
    }

    // Update fullmove counter
    if (currentPlayer == WHITE) {
        fullmoveCounter++;
    }
}

void Position::removeCastlingAbility(Color color, MoveType type) {
    if (color == WHITE) {
        if (type == KINGSIDE_CASTLE || type == NORMAL) {
            // Remove white kingside castling ability
            castlingAbility &= 0b1110;
        }
        if (type == QUEENSIDE_CASTLE  || type == NORMAL) {
            // Remove white queenside castling ability
            castlingAbility &= 0b1101;
        }
    } else {
        if (type == KINGSIDE_CASTLE || type == NORMAL) {
            // Remove kingside castling ability
            castlingAbility &= 0b1011;
        }
        if (type == QUEENSIDE_CASTLE  || type == NORMAL) {
            // Remove queenside castling ability
            castlingAbility &= 0b0111;
        }
    }
}

#endif