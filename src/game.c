#include "includes/game.h"
#include <stdlib.h>
#include <stdio.h>

Game_context initGame(int boardRows, int boardCols, StartingPlayer startingPlayer)
{
    Game_context game;
    game.gameWindow = WELCOME;
    game.boardRows = boardRows;
    game.boardCols = boardCols;
    game.board = NULL;
    game.startingPlayer = startingPlayer;
    game.gameState = PRE_GAME;
    game.winner = EMPTY;
    
    game.board = (BoardState **)malloc(game.boardRows * sizeof(BoardState *));
    for (int i = 0; i < game.boardRows; i++)
    {
        game.board[i] = (BoardState *)malloc(game.boardCols * sizeof(BoardState));
    }
    return game;
}

void freeBoard(Game_context *game)
{
    for (int i = 0; i < game->boardRows; i++)
    {
        free(game->board[i]);
    }
    free(game->board);
}

void changeBoard(Game_context *game, int newBoardRows, int newBoardCols)
{
    freeBoard(game);
    game->boardRows = newBoardRows;
    game->boardCols = newBoardCols;
    game->board = (BoardState **)malloc(game->boardRows * sizeof(BoardState *));
    for (int i = 0; i < game->boardRows; i++)
    {
        game->board[i] = (BoardState *)malloc(game->boardCols * sizeof(BoardState));
    }
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

int getStartPlayer(int startingPlayer)
{
    if (startingPlayer == 0)
    {
        return arc4random() % 2 + 1;
    }
    return startingPlayer;
}

void resetGame(Game_context *game)
{
    freeBoard(game);
    *game = initGame(game->boardRows, game->boardCols, game->startingPlayer);
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
    if (!valid)
        return false;
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

bool undoMove(Game_context *game, int col, BoardState player)
{
    if (col < 0 || col >= game->boardCols)
        return false;
    for (int row = 0; row < game->boardRows; row++)
    {
        if (game->board[row][col] == player)
        {
            game->board[row][col] = EMPTY;
            game->winner = EMPTY;
            game->gameWindow = GAME;
            game->gameState = player == PLAYER ? PLAYER_TURN : PC_TURN;
            return true;
        }
    }
    return false;
}

int checkWin(const Game_context game)
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

void printBoard(Game_context game)
{
    printf("\n");
    printf("|");
    for (int i = 0; i < game.boardCols; i++)
    {
        printf("---|");
    }
    printf("\n");
    for (int i = 0; i < game.boardRows; i++)
    {
        printf("|");
        for (int j = 0; j < game.boardCols; j++)
        {
            printf(" %c |", game.board[i][j] == EMPTY ? ' ' : game.board[i][j] == PLAYER ? 'X'
                                                                                         : '0');
        }
        printf("\n|");
        for (int i = 0; i < game.boardCols; i++)
        {
            printf("---|");
        }
        printf("\n");
    }
    printf("\n");
}

int getRandomValidMove(Game_context *game)
{
    int boardCols = game->boardCols;
    int validMoves[boardCols];
    int validMovesCount = 0;
    for (int col = 0; col < boardCols; col++)
    {
        if (isMoveValid(game, col))
        {
            validMoves[validMovesCount++] = col;
        }
    }
    if (validMovesCount > 0)
    {
        int randomIndex = rand() % validMovesCount;
        return validMoves[randomIndex];
    }
    return -1;
}

int isMoveValid(Game_context *game, int col)
{
    if (col < 0 || col >= game->boardCols)
    {
        return false;
    }

    if (game->board[0][col] != EMPTY)
    {
        return false;
    }

    return true;
}
