#ifndef POSITION_H
#define POSITION_H

#include "../Utils.h"

class Position {
public:
    Position();
    void parseFen(const char* Fen);
    void makeMove(Move move);
    void unmakeMove(Move move, U8 castlingRights, U8 prevEnPassantTargetSquare, U8 prevHalfmoveClock, U8 prevFullmoveCounter);

    // Position accessing
    Piece getPieceAt(int square);
    Color getCurrentPlayer();
    
    U8 getCastlingAbility();
    U8 getEnPassantTarget();
    U8 getHalfmoveClock();
    U8 getFullmoveCounter();

    U64 getOccupiedSquares(Piece piece);
    U64 getOccupiedSquares(Color color);
    U64 getOccupiedSquares();
    U64 getEmptySquares();
    U64 getFriendlyPieces();
    U64 getFriendlyPieces(PieceType pieceType);
    U64 getEnemyPieces();
    U64 getEnemyPieces(PieceType pieceType);
    U64 getPositionHash();

    bool whiteCanKingsideCastle();
    bool whiteCanQueensideCastle();
    bool blackCanKingsideCastle();
    bool blackCanQueensideCastle();

    bool halfmoveClockIsFifty();
    bool isInsufficientMaterial();
    
    int getMaterialScore();

private:
    // Bitboard representation
    U64 bitboards[12]; // A1 = Least significant bit, H8 = Most significant bit

    Color currentPlayer;
    U8 castlingAbility; // (0000 qkQK)
    U8 enPassantTargetSquare;
    U8 halfmoveClock;
    U8 fullmoveCounter;

    // Position hash
    U64 positionHash;
    U64 zobristRandomNumbers[781];
    void initPositionHash();

    // Position FEN parsing
    void initializeBoards();
    Piece charToPiece(char fenChar);
    void parsePiecePlacement(const char* FEN, int& i);
    void parsePlayer(const char* FEN, int& i);
    void parseCastlingAbility(const char* FEN, int& i);
    void parseEnPassantTarget(const char* FEN, int& i);
    void parseHalfmoveClock(const char* FEN, int& i);
    void parseFullmoveCounter(const char* FEN, int& i);

    // Move making and unmaking
    void toggleCapturedPiece(Move move, bool makeMove=true);
    void toggleCastleRooks(MoveType type, bool makeMove=true); 
    void removeCastlingAbility(Color color, MoveType type);

    int materialScore;
    int pieceValue[12] = {1, 3, 3, 5, 9, 10000000, -1, -3, -3, -5, -9, -10000000};
};

#endif