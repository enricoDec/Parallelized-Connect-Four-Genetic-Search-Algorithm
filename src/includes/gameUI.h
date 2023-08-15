#ifndef GAME_UI_H
#define GAME_UI_H

#include "game.h"

/**
 * @file gameUI.h
 * @brief This file contains the game UI
 */

/**
 * @brief The size of the board
 */
typedef struct
{
    int boardRows; /**< Number of rows in the board */
    int boardCols; /**< Number of columns in the board */
} Board_size;

/**
 * Initialize the game UI
 * @param screenWidth The width of the screen
 * @param screenHeight The height of the screen
 */
void initUI(int screenWidth, int screenHeight);

/**
 * The main game loop
 * @param game The game context
 */
void loop(Game_context *game);

/**
 * @brief Draw the welcome window
 * @param game The game context
 * @param prevVisualStyleActive The previous active visual style
 */
void drawWelcomeWindow(Game_context *game, int *prevVisualStyleActive);

/**
 * @brief Draw the game window
 * @param game The game context
 */
void drawGameWindow(Game_context *game);

/**
 * @brief Draw the finish window
 * @param game The game context
 */
void drawFinishWindow(Game_context *game);

/**
 * @brief Draw the board
 * @param game The game context
 * @param cursorX The cursor X position (player column selection), -1 if not visible
 */
void drawBoard(Game_context *game, int cursorX);

/**
 * @brief Draw the coin flip to decide the starting player
 * @param game The game context
 * @return -1 if exit window, else 0
 */
int drawCoinFlip(Game_context *game);

//////////////////////////////////////////
//              Utils                   //
//////////////////////////////////////////

/**
 * @brief Update the style of the UI
 * @param visualStyleActive The active visual style, where:
 * - 0 = default,
 * - 1 = jungle,
 * - 2 = lavanda,
 * - 3 = dark,
 * - 4 = bluish,
 * - 5 = cyber,
 * - 6 = terminal
 * @param prevVisualStyleActive The previous active visual style
 */
void updateStyle(int visualStyleActive, int *prevVisualStyleActive);

/**
 * @brief Update the font of the UI based on the current window size
 * @param screenWidth The width of the screen
 */
void updateFont(int screenWidth);

/**
 * @brief Get the font size for a heading 1
 * @return The heading 1 font size
 */
int getH1Font(void);

/**
 * @brief Get the font size for a heading 2
 * @return The heading 2 font size
 */
int getH2Font(void);

/**
 * @brief Get the font size for body text
 * @return The body text font size
 */
int getBodyFont(void);

#endif // GAME_UI_H
