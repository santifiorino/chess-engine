#ifndef POSITIONMOVE_CPP
#define POSITIONMOVE_CPP

#include "Position.h"
#include <iostream>

void Position::makeMove(Move move){

    Piece movedPiece = getPieceAt(move.from);
    U64 fromSquare = 1ULL << move.from;
    U64 toSquare = 1ULL << move.to;

    toggleCapturedPiece(move);
    toggleCastleRooks(move.type);
    
    
    // Remove the piece from the original square
    bitboards[movedPiece] ^= fromSquare;
    if (move.type != PROMOTION && move.type != PROMOTION_CAPTURE) {
        // Place the piece on the new square
        bitboards[movedPiece] |= toSquare;
    } else {
        // Instead of placing the same piece, place the promoted piece
        bitboards[move.promotion + 6 * currentPlayer] |= toSquare;
    }

    // Update castling ability
    if ((movedPiece == WHITE_KING || movedPiece == BLACK_KING) && move.type == NORMAL) {
        // Remove both castling abilities
        removeCastlingAbility(getCurrentPlayer(), NORMAL);
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

    // Update current player
    currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;

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

void Position::unmakeMove(Move move, U8 castlingRights, U8 prevEnPassantTargetSquare, U8 prevHalfmoveClock, U8 prevFullmoveCounter) {
    Piece movedPiece = getPieceAt(move.to);
    U64 fromSquare = setBit(0ULL, move.from);
    U64 toSquare = setBit(0ULL, move.to);

    toggleCapturedPiece(move, false);
    toggleCastleRooks(move.type, false);

    if (move.type != PROMOTION && move.type != PROMOTION_CAPTURE) {
        // Remove the piece from the new square
        bitboards[movedPiece] ^= toSquare;
        // Put moved piece back where it was
        bitboards[movedPiece] |= fromSquare;
    } else {
        // Remove the promoted piece from the new square
        bitboards[move.promotion + 6 * (1-currentPlayer)] ^= toSquare;
        // Put the pawn back where it was
        bitboards[PAWN + 6 * (1-currentPlayer)] |= fromSquare;;
    }

    // Update current player
    currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;

    // Update castling ability
    castlingAbility = castlingRights;

    // Update en passant target square
    enPassantTargetSquare = prevEnPassantTargetSquare;

    // Update halfmove clock
    halfmoveClock = prevHalfmoveClock;
    
    // Update fullmove counter
    fullmoveCounter = prevFullmoveCounter;

}

void Position::toggleCapturedPiece(Move move, bool makeMove) {
    U64 toSquare = setBit(0ULL, move.to);
    if (move.type == CAPTURE || move.type == PROMOTION_CAPTURE) {
        Piece captured = move.captured;
        bitboards[captured] ^= toSquare;
    } else if (move.type == EN_PASSANT) {
        U8 capturedPawnSquare = move.to + (currentPlayer == WHITE ? -8 : 8) * (makeMove ? 1 : -1);
        Piece capturedPawn = move.captured;
        bitboards[capturedPawn] ^= setBit(0ULL, capturedPawnSquare);
    }
}

void Position::toggleCastleRooks(MoveType type, bool makeMove) {
    Color player = currentPlayer;
    if (!makeMove) player = (player == WHITE) ? BLACK : WHITE;
    if (type == KINGSIDE_CASTLE) {
        // Move the rook
        if (player == WHITE) {
            if (makeMove) removeCastlingAbility(WHITE, NORMAL);
            bitboards[WHITE_ROOK] ^= 1ULL << H1;
            bitboards[WHITE_ROOK] ^= 1ULL << F1;
        } else {
            if (makeMove) removeCastlingAbility(BLACK, NORMAL);
            bitboards[BLACK_ROOK] ^= 1ULL << H8;
            bitboards[BLACK_ROOK] ^= 1ULL << F8;
        }
    } else if (type == QUEENSIDE_CASTLE) {
        // Move the rook
        if (player == WHITE) {
            if (makeMove) removeCastlingAbility(WHITE, NORMAL);
            bitboards[WHITE_ROOK] ^= 1ULL << A1;
            bitboards[WHITE_ROOK] ^= 1ULL << D1;
        } else {
            if (makeMove) removeCastlingAbility(BLACK, NORMAL);
            bitboards[BLACK_ROOK] ^= 1ULL << A8;
            bitboards[BLACK_ROOK] ^= 1ULL << D8;
        }
    }
}

#endif