#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../ChessGame.h"

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
    SDL_Texture* pieceTexture[12];
    int pieceWidth[12], pieceHeight[12];
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
    for (int i = 0; i < 12; i++) {
        SDL_QueryTexture(pieceTexture[i], NULL, NULL, &pieceWidth[i], &pieceHeight[i]);
    }

    int mouseX, mouseY;
    int grabbedPieceSquare = -1;
    U64 grabbedPieceTargets = 0ULL;
    U64 paintedSquares = 0ULL;

    SDL_Event event;
    bool running = true;
    while (running) {
        SDL_GetMouseState(&mouseX, &mouseY);
        while (SDL_PollEvent(&event)) {
            int i = mouseX / 100;
            int j = mouseY / 100;
            int square = (7 - j) * 8 + i;
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // Grab piece
                    grabbedPieceSquare = square;
                    grabbedPieceTargets = game.getMovesFrom(grabbedPieceSquare);
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (grabbedPieceSquare != -1) {
                        // Make move if legal and drop piece
                        int move = game.isLegalMove(grabbedPieceSquare, square);
                        if (move != -1) {
                            game.makeMove(move);
                            paintedSquares = 0ULL;
                        }
                        grabbedPieceSquare = -1;
                    }
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    // Paint square
                    paintedSquares ^= setBit(0ULL, square);
                }
            }
        }

        // Display squares
        SDL_RenderClear(renderer);
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
                        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 70);
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

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}