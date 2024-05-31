#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../ChessGame/ChessGame.h"

const int WHITE_SQUARE_COLOR[3] = {237, 214, 167};
const int BLACK_SQUARE_COLOR[3] = {184, 135, 98};

int main(int argc, char* argv[]) {
    ChessGame game = ChessGame();
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Chess Engine",
        100,
        100,
        800, 800,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Square
    SDL_Rect boardRect;
    boardRect.w = 100;
    boardRect.h = 100;
    // Pieces
    SDL_Rect pieceRect;
    SDL_Texture* pieceTexture[16];
    int pieceWidth[16], pieceHeight[16];
    pieceTexture[0] = IMG_LoadTexture(renderer, "../pieces/white_pawn.png");
    pieceTexture[1] = IMG_LoadTexture(renderer, "../pieces/white_knight.png");
    pieceTexture[2] = IMG_LoadTexture(renderer, "../pieces/white_bishop.png");
    pieceTexture[3] = IMG_LoadTexture(renderer, "../pieces/white_rook.png");
    pieceTexture[4] = IMG_LoadTexture(renderer, "../pieces/white_queen.png");
    pieceTexture[5] = IMG_LoadTexture(renderer, "../pieces/white_king.png");
    pieceTexture[6] = IMG_LoadTexture(renderer, "../pieces/black_pawn.png");
    pieceTexture[7] = IMG_LoadTexture(renderer, "../pieces/black_knight.png");
    pieceTexture[8] = IMG_LoadTexture(renderer, "../pieces/black_bishop.png");
    pieceTexture[9] = IMG_LoadTexture(renderer, "../pieces/black_rook.png");
    pieceTexture[10] = IMG_LoadTexture(renderer, "../pieces/black_queen.png");
    pieceTexture[11] = IMG_LoadTexture(renderer, "../pieces/black_king.png");
    pieceTexture[12] = IMG_LoadTexture(renderer, "../pieces/checkmate_won.png");
    pieceTexture[13] = IMG_LoadTexture(renderer, "../pieces/checkmate_lost.png");
    pieceTexture[14] = IMG_LoadTexture(renderer, "../pieces/tie_white.png");
    pieceTexture[15] = IMG_LoadTexture(renderer, "../pieces/tie_black.png");
    for (int i = 0; i < 16; i++) {
        SDL_QueryTexture(pieceTexture[i], NULL, NULL, &pieceWidth[i], &pieceHeight[i]);
    }

    int mouseX, mouseY;
    int grabbedPieceSquare = -1;
    U64 grabbedPieceTargets = 0ULL;
    U64 paintedSquares = 0ULL;
    Move lastMove;

    SDL_Event event;
    bool running = true;
    
    while (running) {
        SDL_GetMouseState(&mouseX, &mouseY);
        // Handle mouse events (player moves)
        while (SDL_PollEvent(&event)) {
            int i = mouseX / 100;
            int j = mouseY / 100;
            int square = (7 - j) * 8 + i;
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // Grab piece
                    if (game.getPieceAt(square) != NOPIECE) {
                        grabbedPieceSquare = square;
                        grabbedPieceTargets = game.getMovesFrom(grabbedPieceSquare);
                    } else {
                        grabbedPieceSquare = -1;
                    }
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (grabbedPieceSquare != -1) {
                        // Make move if legal and drop piece
                        bool moved = game.makeMove(grabbedPieceSquare, square);
                        if (moved) paintedSquares = 0ULL;
                        grabbedPieceSquare = -1;
                    }
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    // Paint square
                    paintedSquares ^= setBit(0ULL, square);
                }
            }
        }
        // Handle AI moves
        if (!game.isGameOver && game.getCurrentPlayer() == BLACK){
            game.makeAIMove();
        }
        
        SDL_RenderClear(renderer);

        // Display squares
        lastMove = game.getLastMove();
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                boardRect.x = i * 100;
                boardRect.y = j * 100;
                int square = (7 - j) * 8 + i;
                if ((i + j) % 2 == 0) {
                    SDL_SetRenderDrawColor(renderer, WHITE_SQUARE_COLOR[0], WHITE_SQUARE_COLOR[1], WHITE_SQUARE_COLOR[2], 255);
                } else {
                    SDL_SetRenderDrawColor(renderer, BLACK_SQUARE_COLOR[0], BLACK_SQUARE_COLOR[1], BLACK_SQUARE_COLOR[2], 255);
                }
                SDL_RenderFillRect(renderer, &boardRect);
                if (paintedSquares & setBit(0ULL, square)) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
                    SDL_RenderFillRect(renderer, &boardRect);
                }
                if (lastMove.from != NOSQUARE) {
                    if (lastMove.from == square || lastMove.to == square) {
                        boardRect.x = i * 100;
                        boardRect.y = j * 100;
                        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 100);
                        SDL_RenderFillRect(renderer, &boardRect);
                    }
                }
            }
        }

        // Paint attacked squares by the grabbed piece
        if (grabbedPieceSquare != -1) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    int square = (7 - j) * 8 + i;
                    if (grabbedPieceTargets & setBit(0ULL, square)) {
                        boardRect.x = i * 100;
                        boardRect.y = j * 100;
                        SDL_SetRenderDrawColor(renderer, 0, 0, 100, 50);
                        SDL_RenderFillRect(renderer, &boardRect);
                    }
                    
                }
            }
        }
    
        // Display pieces
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int square = (7 - j) * 8 + i;
                int piece = game.getPieceAt(square);
                if (piece != NOPIECE && square != grabbedPieceSquare) {
                    pieceRect.x = i * 100 + (100 - pieceWidth[piece]) / 2;
                    pieceRect.y = j * 100 + (100 - pieceHeight[piece]) / 2;
                    pieceRect.w = pieceWidth[piece];
                    pieceRect.h = pieceHeight[piece];
                    SDL_RenderCopy(renderer, pieceTexture[piece], NULL, &pieceRect);
                }
            }
        }

        // Display grabbed piece
        if (grabbedPieceSquare != -1) {
            int piece = game.getPieceAt(grabbedPieceSquare);
            pieceRect.x = mouseX - pieceWidth[piece] / 2;
            pieceRect.y = mouseY - pieceHeight[piece] / 2;
            pieceRect.w = pieceWidth[piece];
            pieceRect.h = pieceHeight[piece];
            SDL_RenderCopy(renderer, pieceTexture[piece], NULL, &pieceRect);
        }

        // Display game over badges
        if (game.isGameOver) {
            int winner = game.winner;
            int winnerKing = winner == WHITE ? WHITE_KING : BLACK_KING;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    int piece = game.getPieceAt((7 - j) * 8 + i);
                    if (piece == WHITE_KING || piece == BLACK_KING) {
                        int texture = piece == WHITE_KING ? 14 : 15; // Tie texture
                        if (winner != -1)
                            texture = piece == winnerKing ? 12 : 13; // Checkmate texture
                        pieceRect.x = i * 100 + 60;
                        pieceRect.y = j * 100 + 10;
                        pieceRect.w = pieceWidth[texture];
                        pieceRect.h = pieceHeight[texture];
                        SDL_RenderCopy(renderer, pieceTexture[texture], NULL, &pieceRect);
                    }
                }
            }
        } 

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}