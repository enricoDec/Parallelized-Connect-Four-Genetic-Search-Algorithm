#include "includes/game.h"
#include <stdlib.h>

Game_context initGame(int boardRows, int boardCols, int startingPlayer) {
    Game_context game;
    game.boardRows = boardRows;
    game.boardCols = boardCols;
    game.board = (state**)malloc(game.boardRows * sizeof(state*));
    for (int i = 0; i < game.boardRows; i++)
    {
        game.board[i] = (state*)malloc(game.boardCols * sizeof(state));
    }
    game.startingPlayer = startingPlayer;
    
    return game;
}

void endGame(Game_context game) {
    // Clean up the allocated memory
    for (int i = 0; i < game.boardRows; i++)
    {
        free(game.board[i]);
    }
    free(game.board);
}