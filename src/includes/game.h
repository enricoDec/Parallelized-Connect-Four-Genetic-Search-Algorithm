#ifndef GAME_H
#define GAME_H

typedef enum
{
    WELCOME,
    GAME,
    FINISH
} CurrentWindow;

typedef enum
{
    EMPTY = 0,
    PLAYER = 1,
    PC = 2,
} state;

typedef enum
{
    RANDOM = 0,
    USER = 1,
    COMPUTER = 2,
} startingPlayer;

typedef struct
{
    CurrentWindow gameWindow;
    int boardRows;
    int boardCols;
    state **board;
    startingPlayer startingPlayer; // 0: Random, 1: User, 2: PC
} Game_context;

/**
 * @brief Initialize the game context
 * @param boardRows Number of rows in the board
 * @param boardCols Number of columns in the board
 * @param startingPlayer 0: Random, 1: User, 2: PC
*/
Game_context initGame(int boardRows, int boardCols, int startingPlayer);

// end game and cleanup memory
void endGame(Game_context game);

#endif