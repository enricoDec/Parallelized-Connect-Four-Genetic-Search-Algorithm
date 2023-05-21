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
    game.winner = EMPTY;
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

void resetGame(Game_context *game)
{
    endGame(game);
    *game = initGame(game->boardRows, game->boardCols, game->startingPlayer);
}


bool checkMove(Game_context *game)
{
    for (int i = 0; i < game->boardRows; i++)
    {
        for (int j = 0; j < game->boardCols; j++)
        {
            if (game->board[i][j] == EMPTY)
            {
                return true;
            }
        }
    }
    return false;
}

int checkWin(Game_context *game) {
    // Check rows
    for (int i = 0; i < game->boardRows; i++)
    {
        for (int j = 0; j < game->boardCols - 3; j++)
        {
            if (game->board[i][j] != EMPTY && game->board[i][j] == game->board[i][j + 1] && game->board[i][j] == game->board[i][j + 2] && game->board[i][j] == game->board[i][j + 3])
            {
                return game->board[i][j];
            }
        }
    }
    // Check columns
    for (int i = 0; i < game->boardRows - 3; i++)
    {
        for (int j = 0; j < game->boardCols; j++)
        {
            if (game->board[i][j] != EMPTY && game->board[i][j] == game->board[i + 1][j] && game->board[i][j] == game->board[i + 2][j] && game->board[i][j] == game->board[i + 3][j])
            {
                return game->board[i][j];
            }
        }
    }
    // Check diagonals
    for (int i = 0; i < game->boardRows - 3; i++)
    {
        for (int j = 0; j < game->boardCols - 3; j++)
        {
            if (game->board[i][j] != EMPTY && game->board[i][j] == game->board[i + 1][j + 1] && game->board[i][j] == game->board[i + 2][j + 2] && game->board[i][j] == game->board[i + 3][j + 3])
            {
                return game->board[i][j];
            }
        }
    }
    for (int i = 0; i < game->boardRows - 3; i++)
    {
        for (int j = 3; j < game->boardCols; j++)
        {
            if (game->board[i][j] != EMPTY && game->board[i][j] == game->board[i + 1][j - 1] && game->board[i][j] == game->board[i + 2][j - 2] && game->board[i][j] == game->board[i + 3][j - 3])
            {
                return game->board[i][j];
            }
        }
    }
    // Check draw
    if (!checkMove(game))
    {
        return 3;
    }
    return 0;
}
