#ifndef POSITION_H
#define POSITION_H

#include "Utils.h"

class Position {
public:
    Position(char* FEN);

    // Accessing the position
    Piece getPieceAt(int square);
    Color getCurrentPlayer();
    bool whiteCanCastleKingside();
    bool whiteCanCastleQueenside();
    bool blackCanCastleKingside();
    bool blackCanCastleQueenside();
    U8 getEnPassantTarget();
    U8 getHalfmoveClock();
    U8 getFullmoveCounter();

    // Bitboard
    U64 getFriendlyPieces();
    U64 getFriendlyPieces(PieceType pieceType);
    U64 getEnemyPieces();
    U64 getEnemyPieces(PieceType pieceType);
    U64 getOccupiedSquares();
    U64 getEmptySquares();
    
private:
    // Bitboard representation
    U64 bitboards[12]; // A1 = Least significant bit, H8 = Most significant bit

    Color currentPlayer;
    U8 castlingAbility; // (0000 qkQK)
    U8 enPassantTargetSquare;
    U8 halfmoveClock;
    U8 fullmoveCounter;

    // Board initialization
    void initializeBoard();
    Piece charToPiece(char fenChar);
    void parsePiecePlacement(const char* FEN, int& i);
    void parseCastlingAbility(const char* FEN, int& i);
    void parseEnPassantTarget(const char* FEN, int& i);
    void parseHalfmoveClock(const char* FEN, int& i);
    void parseFullmoveCounter(const char* FEN, int& i);

    // Utils
    U64 getBlackOccupiedSquares();
    U64 getWhiteOccupiedSquares();
};

#endif