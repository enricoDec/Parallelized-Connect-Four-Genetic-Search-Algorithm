#include "includes/game.h"
#include "includes/gameUI.h"

int main(void)
{
    initUI(800, 450);
    Game_context game = initGame(7, 6, 0);
    loop(game);
    endGame(game);
}