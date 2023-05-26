#include "includes/game.h"
#include <stdlib.h>
#include <stdio.h>

Game_context initGame(int boardRows, int boardCols, int startingPlayer)
{
    Game_context game;
    game.gameWindow = WELCOME;
    game.boardRows = boardRows;
    game.boardCols = boardCols;
    game.board = (BoardState **)malloc(game.boardRows * sizeof(BoardState *));
    for (int i = 0; i < game.boardRows; i++)
    {
        game.board[i] = (BoardState *)malloc(game.boardCols * sizeof(BoardState));
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
    game->board = (BoardState **)malloc(game->boardRows * sizeof(BoardState *));
    for (int i = 0; i < game->boardRows; i++)
    {
        game->board[i] = (BoardState *)malloc(game->boardCols * sizeof(BoardState));
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

void cleanupGame(Game_context *game)
{
    for (int i = 0; i < game->boardRows; i++)
    {
        free(game->board[i]);
    }
    free(game->board);
}

void resetGame(Game_context *game)
{
    cleanupGame(game);
    *game = initGame(game->boardRows, game->boardCols, RANDOM);
}

bool isBoardFull(Game_context game)
{
    for (int i = 0; i < game.boardRows; i++)
    {
        for (int j = 0; j < game.boardCols; j++)
        {
            if (game.board[i][j] == EMPTY)
            {
                return false;
            }
        }
    }
    return true;
}

bool doMove(Game_context *game, int col, BoardState player)
{
    if (col < 0 || col >= game->boardCols)
        return false;
    bool valid = false;
    for (int row = game->boardRows - 1; row >= 0; row--)
    {
        if (game->board[row][col] == EMPTY)
        {
            game->board[row][col] = player;
            valid = true;
            break;
        }
    }
    int win = checkWin(*game);
    if (win != 0)
    {
        game->gameWindow = FINISH;
        game->winner = win == 1 ? PLAYER : win == 2 ? PC
                                                    : EMPTY;
        return true;
    }
    // change the game state
    if (valid)
    {
        game->gameState = game->gameState == PLAYER_TURN ? PC_TURN : PLAYER_TURN;
    }
    return valid;
}

void undoMove(Game_context *game, int col, BoardState player)
{
    for (int row = 0; row < game->boardRows; row++)
    {
        if (game->board[row][col] == player)
        {
            game->board[row][col] = EMPTY;
            break;
        }
    }
    game->gameState = game->gameState == PLAYER_TURN ? PC_TURN : PLAYER_TURN;
}

int checkWin(Game_context game)
{
    // Check rows
    for (int i = 0; i < game.boardRows; i++)
    {
        for (int j = 0; j < game.boardCols - 3; j++)
        {
            if (game.board[i][j] != EMPTY && game.board[i][j] == game.board[i][j + 1] && game.board[i][j] == game.board[i][j + 2] && game.board[i][j] == game.board[i][j + 3])
            {
                return game.board[i][j];
            }
        }
    }
    // Check columns
    for (int i = 0; i < game.boardRows - 3; i++)
    {
        for (int j = 0; j < game.boardCols; j++)
        {
            if (game.board[i][j] != EMPTY && game.board[i][j] == game.board[i + 1][j] && game.board[i][j] == game.board[i + 2][j] && game.board[i][j] == game.board[i + 3][j])
            {
                return game.board[i][j];
            }
        }
    }
    // Check diagonals
    for (int i = 0; i < game.boardRows - 3; i++)
    {
        for (int j = 0; j < game.boardCols - 3; j++)
        {
            if (game.board[i][j] != EMPTY && game.board[i][j] == game.board[i + 1][j + 1] && game.board[i][j] == game.board[i + 2][j + 2] && game.board[i][j] == game.board[i + 3][j + 3])
            {
                return game.board[i][j];
            }
        }
    }
    for (int i = 0; i < game.boardRows - 3; i++)
    {
        for (int j = 3; j < game.boardCols; j++)
        {
            if (game.board[i][j] != EMPTY && game.board[i][j] == game.board[i + 1][j - 1] && game.board[i][j] == game.board[i + 2][j - 2] && game.board[i][j] == game.board[i + 3][j - 3])
            {
                return game.board[i][j];
            }
        }
    }
    // Check draw (board is full)
    if (isBoardFull(game))
    {
        return 3;
    }
    return 0;
}

Game_context copyGameContext(const Game_context *source)
{
    Game_context copy;
    copy.gameWindow = source->gameWindow;
    copy.boardRows = source->boardRows;
    copy.boardCols = source->boardCols;

    // Allocate memory for the new board
    copy.board = (BoardState **)malloc(copy.boardRows * sizeof(BoardState *));
    for (int i = 0; i < copy.boardRows; i++)
    {
        copy.board[i] = (BoardState *)malloc(copy.boardCols * sizeof(BoardState));
        for (int j = 0; j < copy.boardCols; j++)
        {
            copy.board[i][j] = source->board[i][j];
        }
    }

    copy.startingPlayer = source->startingPlayer;
    copy.gameState = source->gameState;
    copy.winner = source->winner;

    return copy;
}

void printBoard(Game_context game)
{
    for (int i = 0; i < game.boardRows; i++)
    {
        for (int j = 0; j < game.boardCols; j++)
        {
            BoardState state = game.board[i][j];
            if (state == EMPTY)
            {
                printf("  ");
            }
            else if (state == PLAYER)
            {
                printf("X ");
            }
            else if (state == PC)
            {
                printf("O ");
            }
        }
        printf("\n");
    }
    printf("\n");
}
