#ifndef POSITION_H
#define POSITION_H

#include "Utils.h"

class Position {
public:
    Position(char* FEN);

    // Accessing the position
    Piece getPieceAt(int square);
    uint64 getBitboard(Piece piece);
    Color getCurrentPlayer();
    bool whiteCanCastleKingside();
    bool whiteCanCastleQueenside();
    bool blackCanCastleKingside();
    bool blackCanCastleQueenside();
    uint8 getEnPassantTarget();
    uint8 getHalfmoveClock();
    uint8 getFullmoveCounter();

    // Bitboard
    uint64 getBlackOccupiedSquares();
    uint64 getWhiteOccupiedSquares();
    uint64 getOccupiedSquares();
    uint64 getEmptySquares();
    
private:
    // Square centric board representation
    Piece board[64]; // A1 = 0, H8 = 63
    // Bitboard representation
    uint64 bitboards[12]; // A1 = Least significant bit, H8 = Most significant bit

    Color currentPlayer;
    uint8 castlingAbility; // (0000 qkQK)
    uint8 enPassantTargetSquare;
    uint8 halfmoveClock;
    uint8 fullmoveCounter;

    // Board initialization
    void initializeBoard();
    Piece charToPiece(char fenChar);
    void parsePiecePlacement(const char* FEN, int& i);
    void parseCastlingAbility(const char* FEN, int& i);
    void parseEnPassantTarget(const char* FEN, int& i);
    void parseHalfmoveClock(const char* FEN, int& i);
    void parseFullmoveCounter(const char* FEN, int& i);
};

#endif