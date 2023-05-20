#include "includes/game.h"
#include <stdlib.h>

Game_context initGame(int boardRows, int boardCols, int startingPlayer)
{
    Game_context game;
    game.gameWindow = WELCOME;
    game.boardRows = boardRows;
    game.boardCols = boardCols;
    game.board = (boardState **)malloc(game.boardRows * sizeof(boardState *));
    for (int i = 0; i < game.boardRows; i++)
    {
        game.board[i] = (boardState *)malloc(game.boardCols * sizeof(boardState));
    }
    game.startingPlayer = startingPlayer;
    game.gameState = PRE_GAME;
    return game;
}

void changeBoard(Game_context *game, int newBoardRows, int newBoardCols)
{
    for (int i = 0; i < game->boardRows; i++)
    {
        free(game->board[i]);
    }
    free(game->board);
    game->boardRows = newBoardRows;
    game->boardCols = newBoardCols;
    game->board = (boardState **)malloc(game->boardRows * sizeof(boardState *));
    for (int i = 0; i < game->boardRows; i++)
    {
        game->board[i] = (boardState *)malloc(game->boardCols * sizeof(boardState));
    }
}

int getStartPlayer(int startingPlayer)
{
    if (startingPlayer == 0)
    {
        return rand() % 2 + 1;
    }
    return startingPlayer;
}

void endGame(Game_context *game)
{
    for (int i = 0; i < game->boardRows; i++)
    {
        free(game->board[i]);
    }
    free(game->board);
}
