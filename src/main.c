#include "includes/game.h"
#include "includes/gameUI.h"

/**
 * @file main.c
 * @brief This file contains the main function
 */

/**
 * @brief The main function
 * @return 0 on success
 */
int main(void)
{
    initUI(800, 450);
    Game_context game = initGame(7, 6, RANDOM);
    loop(&game);
    freeBoard(&game);
}
