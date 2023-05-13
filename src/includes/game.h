#ifndef GAME_H
#define GAME_H

/**
 * @file game.h
 * @brief This file contains the game logic
 */

/**
 * @brief The current window of the game
 */
typedef enum
{
    WELCOME, /**< The welcome window */
    GAME,    /**< The game window */
    FINISH   /**< The finish window */
} currentWindow;

/**
 * @brief The state of the board
 */
typedef enum
{
    EMPTY = 0,  /**< The board cell is empty */
    PLAYER = 1, /**< The board cell is occupied by the player */
    PC = 2,     /**< The board cell is occupied by the PC */
} state;

/**
 * @brief The starting player
 */
typedef enum
{
    RANDOM = 0,   /**< Randomly choose the starting player */
    USER = 1,     /**< User starts the game */
    COMPUTER = 2, /**< Computer starts the game */
} startingPlayer;

/**
 * @brief The game context
 */
typedef struct
{
    currentWindow gameWindow;      /**< The current window of the game */
    int boardRows;                 /**< Number of rows in the board */
    int boardCols;                 /**< Number of columns in the board */
    state **board;                 /**< The board */
    startingPlayer startingPlayer; /**< The starting player */
} Game_context;

/**
 * @brief Initialize the game context
 * @param boardRows Number of rows in the board
 * @param boardCols Number of columns in the board
 * @param startingPlayer 0: Random, 1: User, 2: PC
 * @return The game context
 */
Game_context initGame(int boardRows, int boardCols, int startingPlayer);

/**
 * @brief End the game and free the memory
 * @param game The game context
 */
void endGame(Game_context game);

#endif