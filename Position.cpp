#include "Position.h"

void Position::initializeBoard() {
    for (int i = 0; i < 12; i++) {
        bitboards[i] = 0;
    }
}

Piece Position::charToPiece(char fenChar) {
    switch (fenChar) {
        case 'P': return WHITE_PAWN;
        case 'N': return WHITE_KNIGHT;
        case 'B': return WHITE_BISHOP;
        case 'R': return WHITE_ROOK;
        case 'Q': return WHITE_QUEEN;
        case 'K': return WHITE_KING;
        case 'p': return BLACK_PAWN;
        case 'n': return BLACK_KNIGHT;
        case 'b': return BLACK_BISHOP;
        case 'r': return BLACK_ROOK;
        case 'q': return BLACK_QUEEN;
        case 'k': return BLACK_KING;
    }
    return NOPIECE;
}

void Position::parsePiecePlacement(const char* FEN, int& i){
    int file = 7;
    int rank = 0;
    while (FEN[i] != ' ') {
        if (FEN[i] == '/') {
            rank = 0;
            file -= 1;
            i++;
            continue;
        }
        if (isDigit(FEN[i])){
            rank += toDigit(FEN[i]);
            i++;
            continue;
        }
        Piece piece = charToPiece(FEN[i]);
        bitboards[piece] |= (1ULL << toBoardIndex(file, rank));
        rank++;
        i++;
    }
}

void Position::parseCastlingAbility(const char* FEN, int& i){
    castlingAbility = 0;
    if (FEN[i] != '-') {
        while (FEN[i] != ' ') {
            switch (FEN[i]) {
                case 'K': castlingAbility |= 1; break;
                case 'Q': castlingAbility |= 2; break;
                case 'k': castlingAbility |= 4; break;
                case 'q': castlingAbility |= 8; break;
            }
            i++;
        }
    } else {
        i++;
    }
}

void Position::parseEnPassantTarget(const char* FEN, int& i){
    enPassantTargetSquare = 64;
    if (FEN[i] == '-') {
        i += 2;
    } else {
        enPassantTargetSquare = toBoardIndex(FEN[i + 1] - '1', FEN[i] - 'a');
        i += 3;
    }
}

void Position::parseHalfmoveClock(const char* FEN, int& i){
    halfmoveClock = 0;
    while (FEN[i] != ' ') {
        halfmoveClock = halfmoveClock * 10 + toDigit(FEN[i]);
        i++;
    }
    i++;
}

void Position::parseFullmoveCounter(const char* FEN, int& i){
    fullmoveCounter = 0;
    while (FEN[i] != '\0') {
        fullmoveCounter = fullmoveCounter * 10 + toDigit(FEN[i]);
        i++;
    }
}

Position::Position(char* FEN) {
    initializeBoard();
    int i = 0;
    parsePiecePlacement(FEN, i);
    i++;
    currentPlayer = (FEN[i] == 'w') ? WHITE : BLACK;
    i += 2;
    parseCastlingAbility(FEN, i);
    i++;
    parseEnPassantTarget(FEN, i);
    parseHalfmoveClock(FEN, i);
    parseFullmoveCounter(FEN, i);
}

Piece Position::getPieceAt(int square) {
    if (bitboards[WHITE_PAWN] & setBit(0ULL, square)) return WHITE_PAWN;
    if (bitboards[WHITE_KNIGHT] & setBit(0ULL, square)) return WHITE_KNIGHT;
    if (bitboards[WHITE_BISHOP] & setBit(0ULL, square)) return WHITE_BISHOP;
    if (bitboards[WHITE_ROOK] & setBit(0ULL, square)) return WHITE_ROOK;
    if (bitboards[WHITE_QUEEN] & setBit(0ULL, square)) return WHITE_QUEEN;
    if (bitboards[WHITE_KING] & setBit(0ULL, square)) return WHITE_KING;

    if (bitboards[BLACK_PAWN] & setBit(0ULL, square)) return BLACK_PAWN;
    if (bitboards[BLACK_KNIGHT] & setBit(0ULL, square)) return BLACK_KNIGHT;
    if (bitboards[BLACK_BISHOP] & setBit(0ULL, square)) return BLACK_BISHOP;
    if (bitboards[BLACK_ROOK] & setBit(0ULL, square)) return BLACK_ROOK;
    if (bitboards[BLACK_QUEEN] & setBit(0ULL, square)) return BLACK_QUEEN;

    return NOPIECE;
}

Color Position::getCurrentPlayer() {
    return currentPlayer;
}

bool Position::whiteCanCastleKingside() {
    return castlingAbility & 1;
}

bool Position::whiteCanCastleQueenside() {
    return castlingAbility & 2;
}

bool Position::blackCanCastleKingside() {
    return castlingAbility & 4;
}

bool Position::blackCanCastleQueenside() {
    return castlingAbility & 8;
}

U8 Position::getEnPassantTarget() {
    return enPassantTargetSquare;
}

U8 Position::getHalfmoveClock() {
    return halfmoveClock;
}

U8 Position::getFullmoveCounter() {
    return fullmoveCounter;
}

U64 Position::getOccupiedSquares(Piece piece) {
    return bitboards[piece];
}

U64 Position::getOccupiedSquares(Color color) {
    if (color == WHITE) {
        return  getOccupiedSquares(WHITE_PAWN)   |
                getOccupiedSquares(WHITE_KNIGHT) |
                getOccupiedSquares(WHITE_BISHOP) |
                getOccupiedSquares(WHITE_ROOK)   |
                getOccupiedSquares(WHITE_QUEEN)  |
                getOccupiedSquares(WHITE_KING);
    } else {
        return  getOccupiedSquares(BLACK_PAWN)   |
                getOccupiedSquares(BLACK_KNIGHT) |
                getOccupiedSquares(BLACK_BISHOP) |
                getOccupiedSquares(BLACK_ROOK)   |
                getOccupiedSquares(BLACK_QUEEN)  |
                getOccupiedSquares(BLACK_KING);
    
    }
}

U64 Position::getOccupiedSquares() {
    return getOccupiedSquares(WHITE) | getOccupiedSquares(BLACK);
}

U64 Position::getEmptySquares() {
    return ~getOccupiedSquares();
}


U64 Position::getFriendlyPieces() {
    return currentPlayer == WHITE ? getOccupiedSquares(WHITE) : getOccupiedSquares(BLACK);
}

U64 Position::getFriendlyPieces(PieceType pieceType) {
    return bitboards[pieceType + 6 * currentPlayer];
}

U64 Position::getEnemyPieces() {
    return currentPlayer == WHITE ? getOccupiedSquares(BLACK) : getOccupiedSquares(WHITE);
}

U64 Position::getEnemyPieces(PieceType pieceType) {
    return bitboards[pieceType + 6 * (1 - currentPlayer)];
}