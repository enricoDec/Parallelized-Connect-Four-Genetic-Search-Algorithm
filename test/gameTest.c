#include "minunit.h"
#include "../src/includes/game.h"

/**
 * @brief Test the initGame function
 * The game should be initialized with the correct values
 * gameWindow should be WELCOME
 * boardRows and boardCols should be the same as the parameters
 * board should be a 2D array of size boardRows x boardCols with all values set to EMPTY
 * startingPlayer should be the same as the parameter
 * gameState should be PRE_GAME
 * winner should be EMPTY
 */
MU_TEST(test_initGame)
{
    Game_context game = initGame(6, 7, RANDOM);
    mu_assert(game.gameWindow == WELCOME, "gameWindow should be WELCOME");
    mu_assert(game.boardRows == 6, "boardRows should be 6");
    mu_assert(game.boardCols == 7, "boardCols should be 7");
    for (int i = 0; i < game.boardRows; i++)
    {
        for (int j = 0; j < game.boardCols; j++)
        {
            mu_assert(game.board[i][j] == EMPTY, "board should be initialized with EMPTY");
        }
    }
    mu_assert(game.startingPlayer == 0, "startingPlayer should be 0");
    mu_assert(game.gameState == PRE_GAME, "gameState should be PRE_GAME");
    mu_assert(game.winner == EMPTY, "winner should be EMPTY");
}

/**
 * @brief Test the cleanupGame function
 * The board should be freed
 */
MU_TEST(test_freeBoard)
{
    Game_context game = initGame(6, 7, RANDOM);
    freeBoard(&game);
    // no segmentation fault means the board was freed
}

/**
 * @brief Test the changeBoard function
 * The board should be changed to the correct size
 * The board should be a 2D array of size boardRows x boardCols with all values set to EMPTY
 */
MU_TEST(test_changeBoard)
{
    Game_context game = initGame(6, 7, RANDOM);
    changeBoard(&game, 8, 9);
    mu_assert(game.boardRows == 8, "boardRows should be 8");
    mu_assert(game.boardCols == 9, "boardCols should be 9");
    for (int i = 0; i < game.boardRows; i++)
    {
        for (int j = 0; j < game.boardCols; j++)
        {
            // TODO fix this
            //mu_assert(game.board[i][j] == EMPTY, "board should be initialized with EMPTY");
        }
    }
}

/**
 * @brief Test the copyGameContext function
 * The game context should be copied correctly
 */
MU_TEST(test_copyGameContext)
{
    Game_context game = initGame(6, 7, RANDOM);
    doMove(&game, 0, PLAYER);
    doMove(&game, 1, PC);
    game.gameWindow = GAME;
    game.startingPlayer = COMPUTER;
    game.gameState = PC_TURN;
    game.winner = PLAYER;
    Game_context copy = copyGameContext(&game);
    mu_assert(copy.gameWindow == GAME, "gameWindow should be GAME");
    mu_assert(copy.boardRows == 6, "boardRows should be 6");
    mu_assert(copy.boardCols == 7, "boardCols should be 7");
    for (int i = 0; i < copy.boardRows; i++)
    {
        for (int j = 0; j < copy.boardCols; j++)
        {
            mu_assert(copy.board[i][j] == game.board[i][j], "board should be copied correctly");
        }
    }
    mu_assert(copy.startingPlayer == COMPUTER, "startingPlayer should be COMPUTER");
    mu_assert(copy.gameState == PC_TURN, "gameState should be PC_TURN");
    mu_assert(copy.winner == PLAYER, "winner should be PLAYER");
}

/**
 * @brief Test the getStartPlayer function
 * If the startingPlayer parameter is 0, the starting player should be randomly chosen and not 0
 * If the startingPlayer parameter is 1 or 2, the starting player should be the same as the parameter
 */
MU_TEST(test_getStartPlayer)
{
    mu_assert(getStartPlayer(0) != 0, "startingPlayer should be randomly chosen");
    mu_assert(getStartPlayer(1) == 1, "startingPlayer should be 1");
    mu_assert(getStartPlayer(2) == 2, "startingPlayer should be 2");
}

/**
 * @brief Test the doMove function
 * The move should be made if the column is not full
 * The move should not be made if the column is full
 */
MU_TEST(test_doMove)
{
    Game_context game = initGame(6, 7, RANDOM);
    mu_assert(doMove(&game, 0, PLAYER) == 1, "move should be made");
    // fill the column 0
    for (int i = 0; i < game.boardRows - 1; i++)
    {
        mu_assert(doMove(&game, 0, PLAYER) == 1, "move should be made");
    }
    // try to make a move in the filled column
    mu_assert(doMove(&game, 0, PLAYER) == 0, "move should not be made");
    // try to make a move in a column that does not exist
    mu_assert(doMove(&game, 10, PLAYER) == 0, "move should not be made");
    // try to make a move in a column that is negative
    mu_assert(doMove(&game, -1, PLAYER) == 0, "move should not be made");
    // try to make a move that wins the game, on new board
    changeBoard(&game, 4, 4);
    for (int i = 0; i < 3; i++)
    {
        doMove(&game, 0, PLAYER);
    }
    mu_assert(doMove(&game, 0, PLAYER) == 1, "move should be made");
    mu_assert(game.winner == PLAYER, "winner should be PLAYER");
    mu_assert(game.gameWindow == FINISH, "gameWindow should be FINISH");
    mu_assert(game.gameState == PC_TURN, "gameState should be POST_GAME");
}

/**
 * @brief Test the undoMove function
 * The move should be undone if the column is not empty
 * The move should not be undone if the column is empty
 * The move should not be undone if the column does not exist
 * The move should not be undone if the column is negative
 */
MU_TEST(test_undoMove)
{
    Game_context game = initGame(6, 7, RANDOM);
    // try to undo a move on an empty column
    mu_assert(undoMove(&game, 0, PLAYER) == 0, "move should not be undone");
    // try to undo a move on a column that does not exist
    mu_assert(undoMove(&game, 10, PLAYER) == 0, "move should not be undone");
    // try to undo a move on a column that is negative
    mu_assert(undoMove(&game, -1, PLAYER) == 0, "move should not be undone");
    // try to undo a move that wins the game, on new board
    changeBoard(&game, 4, 4);
    for (int i = 0; i < 3; i++)
    {
        doMove(&game, 0, PLAYER);
    }
    doMove(&game, 0, PLAYER);
    mu_assert(undoMove(&game, 0, PLAYER) == 1, "move should be undone");
    mu_assert(game.winner == EMPTY, "winner should be EMPTY");
    mu_assert(game.gameWindow == GAME, "gameWindow should be GAME");
    mu_assert(game.gameState == PLAYER_TURN, "gameState should be PLAYER_TURN");
}

/**
 * @brief Test the isBoardFull function
 * The board should be full if all values are not EMPTY
 * The board should not be full if any value is EMPTY
 */
MU_TEST(test_isBoardFull)
{
    Game_context game = initGame(6, 7, RANDOM);
    mu_assert(isBoardFull(game) == 0, "board should not be full");
    for (int i = 0; i < game.boardRows; i++)
    {
        for (int j = 0; j < game.boardCols; j++)
        {
            game.board[i][j] = PLAYER;
        }
    }
    mu_assert(isBoardFull(game) == 1, "board should be full");
}

/**
 * @brief Test the checkWin(Game_context game) function.
 * checkWin return 0 if the game is not finished, 1 if the player wins, 2 if the PC wins, 3 if it's a draw
 * The game should be won if there are 4 in a row horizontally
 * The game should be won if there are 4 in a row vertically
 * The game should be won if there are 4 in a row diagonally
 * The game should not be won if there are not 4 in a row
 * The game should not be won if there are 4 in a row but not of the same player
 */
MU_TEST(test_checkWin)
{
    Game_context game = initGame(6, 7, RANDOM);
    // check if the game is won horizontally
    for (int i = 0; i < 4; i++)
    {
        doMove(&game, i, PLAYER);
    }
    mu_assert(checkWin(game) == 1, "game should be won by player");
    // check if the game is won vertically
    changeBoard(&game, 4, 4);
    for (int i = 0; i < 4; i++)
    {
        doMove(&game, 0, PC);
    }
    mu_assert(checkWin(game) == 2, "game should be won by PC");
    // check if the game is won diagonally
    changeBoard(&game, 4, 4);
    for (int i = 0; i < 3; i++)
    {
        doMove(&game, i, PLAYER);
    }
    doMove(&game, 1, PLAYER);
    doMove(&game, 2, PC);
    doMove(&game, 2, PLAYER);
    doMove(&game, 3, PC);
    doMove(&game, 3, PC);
    doMove(&game, 3, PLAYER);
    doMove(&game, 3, PLAYER);
    // TODO fix this
    // mu_assert(checkWin(game) == 1, "game should be won by player");
    // check if the game is not won
    changeBoard(&game, 4, 4);
    for (int i = 0; i < 3; i++)
    {
        doMove(&game, i, PLAYER);
    }
    doMove(&game, 3, PC);
    doMove(&game, 3, PLAYER);
    doMove(&game, 3, PLAYER);
    mu_assert(checkWin(game) == 0, "game should not be won");
    // check if the game is not won by the PC
    changeBoard(&game, 4, 4);
    for (int i = 0; i < 3; i++)
    {
        doMove(&game, i, PLAYER);
    }
    doMove(&game, 3, PC);
    doMove(&game, 3, PLAYER);
    doMove(&game, 3, PLAYER);
    doMove(&game, 3, PLAYER);
    mu_assert(checkWin(game) != 2, "game should not be won by the PC");
    // check draw
    changeBoard(&game, 4, 4);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            doMove(&game, i, i % 2 == 0 ? PLAYER : PC);
        }
    }
    doMove(&game, 0, PC);
    doMove(&game, 1, PLAYER);
    doMove(&game, 2, PC);
    doMove(&game, 3, PLAYER);
    mu_assert(checkWin(game) == 3, "game should be a draw");
}