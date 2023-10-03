#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

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
    SETTINGS, /**< The settings window */
    GAME,    /**< The game window */
    FINISH   /**< The finish window */
} CurrentWindow;

/**
 * @brief The BoardState of the board
 */
typedef enum
{
    EMPTY = 0,  /**< The board cell is empty */
    PLAYER = 1, /**< The board cell is occupied by the player */
    PC = 2,     /**< The board cell is occupied by the PC */
} BoardState;

/**
 * @brief The game state, used to determine the current player.
 */
typedef enum
{
    PRE_GAME = 0,    /**< The game is not started yet */
    PLAYER_TURN = 1, /**< The player's turn */
    PC_TURN = 2,     /**< The PC's turn */
} GameState;

/**
 * @brief The starting player
 */
typedef enum
{
    RANDOM = 0,   /**< Randomly choose the starting player */
    USER = 1,     /**< User starts the game */
    COMPUTER = 2, /**< Computer starts the game */
} StartingPlayer;

/**
 * @brief The game context
 */
typedef struct
{
    CurrentWindow gameWindow;      /**< The current window of the game */
    int boardRows;                 /**< Number of rows in the board */
    int boardCols;                 /**< Number of columns in the board */
    BoardState **board;            /**< The board */
    StartingPlayer startingPlayer; /**< The starting player */
    GameState gameState;           /**< The game state */
    BoardState winner;             /**< The winner of the game */
} Game_context;

/**
 * @brief Initialize the game context, be sure to call freeBoard(Game_context *game) to free the memory
 * @param boardRows Number of rows in the board
 * @param boardCols Number of columns in the board
 * @param startingPlayer 0: Random, 1: User, 2: PC
 * @return The game context
 */
Game_context initGame(int boardRows, int boardCols, StartingPlayer startingPlayer);

/**
 * @brief Free the board
 * @param game The game context
 */
void freeBoard(Game_context *game);

/**
 * @brief Change the board size
 * @param game The game context
 * @param newBoardRows The new number of rows in the board
 * @param newBoardCols The new number of columns in the board
 * @note This function will free the memory of the old board
 */
void changeBoard(Game_context *game, int newBoardRows, int newBoardCols);

/**
 * @brief Copy the game context
 * @param source The source game context
 * @return The copied game context
 */
Game_context copyGameContext(const Game_context *source);

/**
 * @brief Get the starting player, picks randomly if the starting player is RANDOM
 * @param startingPlayer 0: Random, 1: User, 2: PC
 * @return The starting player (1: User, 2: PC)
 */
int getStartPlayer(int startingPlayer);

/**
 * @brief Do the move and update the game state, return false if the move is invalid
 * @param game The game context
 * @param col The column to place the piece
 * @param player The player to place the piece
 * @return true if the move is valid, false otherwise
 */
bool doMove(Game_context *game, int col, BoardState player);

/**
 * @brief Undo the move for the given column and player.
 * The winning state will be reset.
 * The current player will be set to the player who made the last move.
 * @param game The game context
 * @param col The column to undo the move
 * @param player The player to undo the move
 * @return true if the move has been undone, false otherwise
 */
bool undoMove(Game_context *game, int col, BoardState player);

/**
 * @brief Check if the board is full
 * @param game The game context
 * @return true if the board is full, false otherwise
 */
bool isBoardFull(Game_context game);

/**
 * @brief Check if the game is finished
 * @param game The game context
 * @return 0 if the game is not finished, 1 if the player wins, 2 if the PC wins, 3 if it's a draw
 */
int checkWin(Game_context game);

/**
 * @brief Reset the game
 * @param game The game context
 */
void resetGame(Game_context *game);

/**
 * @brief Print the board, used for debugging.
 * @param game The game context
 */
void printBoard(Game_context game);

/**
 * @brief Get a random valid move
 * @param game The game context
 * @return The column of the random valid move or -1 if there is no valid move
 */
int getRandomValidMove(Game_context *game);

/**
 * @brief Check if the move is valid
 * @param game The game context
 * @param col The column to place the piece
 * @return true if the move is valid, false otherwise
 */
int isMoveValid(Game_context *game, int col);

#endif // GAME_H
