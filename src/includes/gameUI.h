#ifndef GAME_UI_H
#define GAME_UI_H

#include "game.h"

/**
 * @file gameUI.h
 * @brief This file contains the game UI
 */

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
void loop(Game_context game);

#endif