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
 * @brief The boardState of the board
 */
typedef enum
{
    EMPTY = 0,  /**< The board cell is empty */
    PLAYER = 1, /**< The board cell is occupied by the player */
    PC = 2,     /**< The board cell is occupied by the PC */
} boardState;

/**
 * @brief The game state
 */
typedef enum
{
    PRE_GAME = 0,    /**< The game is not started yet */
    PLAYER_TURN = 1, /**< The player's turn */
    PC_TURN = 2,     /**< The PC's turn */
} gameState;

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
    boardState **board;            /**< The board */
    startingPlayer startingPlayer; /**< The starting player */
    gameState gameState;           /**< The game state */
} Game_context;

/**
 * @brief Initialize the game context, be sure to call endGame(Game_context *game) to free the memory
 * @param boardRows Number of rows in the board
 * @param boardCols Number of columns in the board
 * @param startingPlayer 0: Random, 1: User, 2: PC
 * @return The game context
 */
Game_context initGame(int boardRows, int boardCols, int startingPlayer);

/**
 * @brief Change the board size
 * @param game The game context
 * @param newBoardRows The new number of rows in the board
 * @param newBoardCols The new number of columns in the board
 * @note This function will free the memory of the old board
 */
void changeBoard(Game_context *game, int newBoardRows, int newBoardCols);

/**
 * @brief Get the starting player
 * @param startingPlayer 0: Random, 1: User, 2: PC
 * @return The starting player (1: User, 2: PC)
 */
int getStartPlayer(int startingPlayer);

/**
 * @brief End the game and free the memory
 * @param game The game context
 */
void endGame(Game_context *game);

#endif // GAME_H
