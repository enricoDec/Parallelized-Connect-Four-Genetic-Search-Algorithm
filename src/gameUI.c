#include "includes/gameUI.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include "includes/raygui.h" // TODO: fix include
#pragma GCC diagnostic pop
#include "includes/geneticSearch.h"
#include <string.h>

// Global variables
const bool isDebug = true;
const bool memeMode = true;
GeneticSearchParameters geneticSearchParameters = {1000, 0.8, 0.01, 100, 3};

/**
 * @brief Draw an animated gif
 * @param frameCounter The general frame counter
 * @param currentAnimFrame The current animation frame to load and draw
 * @param frameDelay The frame delay to switch between animation frames
 * @param animFrames The number of animation frames
 * @param nextFrameDataOffset The next frame data offset
 * @param gifTexture The texture of the gif
 * @param gifImg The image of the gif
 * @param posX The x position of the gif
 * @param posY The y position of the gif
 */
void drawAnimatedGif(int *frameCounter, int *currentAnimFrame, int frameDelay, int animFrames, unsigned int nextFrameDataOffset, Texture2D gifTexture, Image gifImg, int posX, int posY);

void initUI(int screenWidth, int screenHeight)
{
    InitWindow(screenWidth, screenHeight, "Connect Four");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetWindowState(FLAG_WINDOW_RESIZABLE);
}

void loop(Game_context *game)
{
    GuiLoadStyleDefault();
    updateFont(GetScreenWidth());
    int prevVisualStyleActive = 0;

    // Main game loop
    while (!WindowShouldClose())
    {
        switch (game->gameWindow)
        {
        case WELCOME:
            drawWelcomeWindow(game, &prevVisualStyleActive);
            break;
        case SETTINGS:
            drawGenSearchSettingWindow(game);
            break;
        case GAME:
            // flip coin if starting player is random
            if (game->startingPlayer == 0)
                if (drawCoinFlip(game) == -1)
                    return;
            game->gameState = game->startingPlayer == 1 ? PLAYER_TURN : PC_TURN;
            drawGameWindow(game);
            break;
        case FINISH:
            drawFinishWindow(game);
            break;
        default:
            exit(1);
            break;
        }
    }
}

void drawWelcomeWindow(Game_context *game, int *prevVisualStyleActive)
{
    const char *WELCOME_LABEL = "Welcome to Connect Four!";
    const char *START_BUTTON_LABEL = "Start";
    const char *SETTINGS_BUTTON_LABEL = "Genetic Search Settings";
    const char *SETTINGS_TITLE_LABEL = "Settings";
    const char *SELECT_BOARD_SIZE_LABEL = "Select Board Size:";
    const char *SELECT_START_PLAYER_LABEL = "Select Starting Player:";
    const char *CHOOSE_STYLE_LABEL = "Choose Style:";
    int screenWidth;
    int screenHeight;
    // Define controls variables
    bool isEditingBoardSizeDropdown = false;
    // if board size is changed, remember to string in the combobox: ~line 150
    Board_size boardSizes[] = {{6, 7}, {9, 7}, {15, 15}, {20, 20}};
    int selectedBoardSize = 0;
    for (int i = 0; i < 4; i++)
    {
        if (game->boardRows == boardSizes[i].boardRows && game->boardCols == boardSizes[i].boardCols)
        {
            selectedBoardSize = i;
            break;
        }
    }
    bool isEditingStartPlayerDropdown = false;
    int selectedStartPlayer = 0; // 0 = Random (default), 1 = User, 2 = PC
    if (game->startingPlayer == 1)
        selectedStartPlayer = 1;
    else if (game->startingPlayer == 2)
        selectedStartPlayer = 2;
    // style
    int visualStyleActive = 0;
    while (!WindowShouldClose())
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        updateFont(screenWidth);
        if (isEditingBoardSizeDropdown || isEditingStartPlayerDropdown)
            GuiLock();
        if (isDebug)
            DrawFPS(screenWidth - 80, 0);
        if (IsKeyPressed(KEY_ENTER))
        {
            game->gameWindow = GAME;
            return;
        }
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        // Welcome
        DrawText(WELCOME_LABEL, screenWidth * 0.07f, screenHeight * 0.08f, getH1Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        GuiLabel((Rectangle){screenWidth * 0.07f, screenHeight * 0.83f, screenWidth * 0.18f, screenHeight * 0.05f}, CHOOSE_STYLE_LABEL);
        if (GuiButton((Rectangle){screenWidth * 0.07f, screenHeight * 0.26f, screenWidth * 0.2f, screenHeight * 0.1f}, START_BUTTON_LABEL))
        {
            game->gameWindow = GAME;
            return;
        }
        if (GuiButton((Rectangle){screenWidth * 0.07f, screenHeight * 0.42f, screenWidth * 0.2f, screenHeight * 0.05f}, SETTINGS_BUTTON_LABEL))
        {
            game->gameWindow = SETTINGS;
            return;
        }
        visualStyleActive = GuiComboBox((Rectangle){screenWidth * 0.07f, screenHeight * 0.88f, screenWidth * 0.18f, screenHeight * 0.07f}, "Default;Jungle;Lavanda;Dark;Bluish;Cyber;Terminal", visualStyleActive);
        // Settings
        DrawText(SETTINGS_TITLE_LABEL, screenWidth * 0.52f, screenHeight * 0.08f, getH2Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        // Controls
        DrawText("Controls:", screenWidth * 0.52f, screenHeight * 0.60f, getH2Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        DrawText("Move cursor:", screenWidth * 0.52f, screenHeight * 0.65f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        Color isRightPressedColor = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT) ? GOLD : GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
        Color isLeftPressedColor = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) ? GOLD : GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
        Color isSpacePressedColor = IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_DOWN) ? GOLD : GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
        // move right
        DrawText("D", screenWidth * 0.54f, screenHeight * 0.68f, getBodyFont(), isRightPressedColor);
        DrawText(" or ", screenWidth * 0.54f + MeasureText("D", getBodyFont()), screenHeight * 0.68f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        DrawText("->", screenWidth * 0.54f + MeasureText("D or ", getBodyFont()), screenHeight * 0.68f, getBodyFont(), isRightPressedColor);
        DrawText(" to move to the right", screenWidth * 0.54f + MeasureText("D or ->", getBodyFont()), screenHeight * 0.68f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        // move left
        DrawText("A", screenWidth * 0.54f, screenHeight * 0.71f, getBodyFont(), isLeftPressedColor);
        DrawText(" or ", screenWidth * 0.54f + MeasureText("A", getBodyFont()), screenHeight * 0.71f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        DrawText("<-", screenWidth * 0.54f + MeasureText("A or ", getBodyFont()), screenHeight * 0.71f, getBodyFont(), isLeftPressedColor);
        DrawText(" to move to the left", screenWidth * 0.54f + MeasureText("A or <-", getBodyFont()), screenHeight * 0.71f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        // General
        DrawText("General:", screenWidth * 0.52f, screenHeight * 0.74f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        // enter move
        DrawText("Space", screenWidth * 0.54f, screenHeight * 0.77f, getBodyFont(), isSpacePressedColor);
        DrawText(" or ", screenWidth * 0.54f + MeasureText("Space", getBodyFont()), screenHeight * 0.77f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        DrawText("v", screenWidth * 0.54f + MeasureText("Space or ", getBodyFont()), screenHeight * 0.77f, getBodyFont(), isSpacePressedColor);
        DrawText(" to choose the column", screenWidth * 0.54f + MeasureText("Space or v", getBodyFont()), screenHeight * 0.77f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        // exit
        DrawText("Esc to quit the game", screenWidth * 0.54f, screenHeight * 0.80f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        // Dropdowns (must be drawn at the end)
        DrawText(SELECT_START_PLAYER_LABEL, screenWidth * 0.52f, screenHeight * 0.37f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        if (GuiDropdownBox((Rectangle){screenWidth * 0.52f, screenHeight * 0.40f, screenWidth * 0.18f, screenHeight * 0.07f}, "Random;User;PC", &selectedStartPlayer, isEditingStartPlayerDropdown))
        {
            game->startingPlayer = selectedStartPlayer;
            isEditingStartPlayerDropdown = !isEditingStartPlayerDropdown;
        }
        DrawText(SELECT_BOARD_SIZE_LABEL, screenWidth * 0.52f, screenHeight * 0.21f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        if (GuiDropdownBox((Rectangle){screenWidth * 0.52f, screenHeight * 0.24f, screenWidth * 0.18f, screenHeight * 0.07f}, "6x7;9x7;15x15;20x20", &selectedBoardSize, isEditingBoardSizeDropdown))
        {
            int newBoardRows = boardSizes[selectedBoardSize].boardRows;
            int newboardCols = boardSizes[selectedBoardSize].boardCols;
            changeBoard(game, newBoardRows, newboardCols);
            isEditingBoardSizeDropdown = !isEditingBoardSizeDropdown;
        }
        updateStyle(visualStyleActive, prevVisualStyleActive);
        GuiUnlock();
        EndDrawing();
    }
    CloseWindow();
}

void drawGenSearchSettingWindow(Game_context *game)
{
    int screenWidth;
    int screenHeight;
    const char *POPULATION_SIZE_LABEL = "Population size:";
    int *populationSize = malloc(sizeof(int));
    *populationSize = 1000;
    bool isEditingPopulationSize = false;
    const char *CROSSOVER_RATE_LABEL = "Crossover rate:";
    float *crossoverRate = malloc(sizeof(float));
    *crossoverRate = 0.8;
    const char *MUTATION_RATE_LABEL = "Mutation rate:";
    float *mutationRate = malloc(sizeof(float));
    *mutationRate = 0.1;
    const char *MAX_GENERATIONS_LABEL = "Max Generations:";
    int *maxGenerations = malloc(sizeof(int));
    *maxGenerations = 100;
    bool isEditingMaxGenerations = false;
    const char *MAX_MOVES_LABEL = "Individual genes (moves) count:";
    int *maxMoves = malloc(sizeof(int));
    *maxMoves = 3;
    bool isEditingMaxMoves = false;
    while (!WindowShouldClose())
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        updateFont(screenWidth);
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        // Center Text with Text Genetic Search Settings
        DrawText("Genetic Search Settings", screenWidth / 2 - MeasureText("Genetic Search Settings", getH1Font()) / 2, screenHeight * 0.08f, getH1Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        // Population size
        DrawText(POPULATION_SIZE_LABEL, screenWidth * 0.03f, screenHeight * 0.25f, getH2Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        if (GuiValueBox((Rectangle){screenWidth * 0.03f, screenHeight * 0.3f, screenWidth * 0.18f, screenHeight * 0.04f}, NULL, populationSize, 1, 10000, isEditingPopulationSize))
            isEditingPopulationSize = !isEditingPopulationSize;
        // Max generations
        DrawText(MAX_GENERATIONS_LABEL, screenWidth * 0.03f, screenHeight * 0.41f, getH2Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        if (GuiValueBox((Rectangle){screenWidth * 0.03f, screenHeight * 0.46f, screenWidth * 0.18f, screenHeight * 0.04f}, NULL, maxGenerations, 1, 10000, isEditingMaxGenerations))
            isEditingMaxGenerations = !isEditingMaxGenerations;
        // Max moves
        DrawText(MAX_MOVES_LABEL, screenWidth * 0.03f, screenHeight * 0.57f, getH2Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        if (GuiValueBox((Rectangle){screenWidth * 0.03f, screenHeight * 0.62f, screenWidth * 0.18f, screenHeight * 0.04f}, NULL, maxMoves, 1, 10000, isEditingMaxMoves))
            isEditingMaxMoves = !isEditingMaxMoves;

        // Crossover rate
        DrawText(CROSSOVER_RATE_LABEL, screenWidth * 0.80f, screenHeight * 0.25f, getH2Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        *crossoverRate = GuiSlider((Rectangle){screenWidth * 0.80f, screenHeight * 0.33f, screenWidth * 0.18f, screenHeight * 0.02f}, NULL, NULL, *crossoverRate, 0.0f, 1.0f);
        DrawText(TextFormat("%.2f", *crossoverRate), screenWidth * 0.79f - MeasureText(TextFormat("%.2f", *crossoverRate), getBodyFont()), screenHeight * 0.33f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        // Mutation rate
        DrawText(MUTATION_RATE_LABEL, screenWidth * 0.80f, screenHeight * 0.41f, getH2Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        *mutationRate = GuiSlider((Rectangle){screenWidth * 0.80f, screenHeight * 0.49f, screenWidth * 0.18f, screenHeight * 0.02f}, NULL, NULL, *mutationRate, 0.0f, 1.0f);
        DrawText(TextFormat("%.2f", *mutationRate), screenWidth * 0.79f - MeasureText(TextFormat("%.2f", *mutationRate), getBodyFont()), screenHeight * 0.49f, getBodyFont(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));

        // Button bottom center to go back
        if (GuiButton((Rectangle){screenWidth / 2 - 100, screenHeight * 0.85f, 200, 50}, "Save and go back"))
        {
            geneticSearchParameters.populationSize = *populationSize;
            geneticSearchParameters.crossoverRate = *crossoverRate;
            geneticSearchParameters.mutationRate = *mutationRate;
            geneticSearchParameters.maxGenerations = *maxGenerations;
            geneticSearchParameters.maxMoves = *maxMoves;
            game->gameWindow = WELCOME;
            return;
        }
        EndDrawing();
    }
    free(populationSize);
    free(crossoverRate);
    free(mutationRate);
    free(maxGenerations);
    free(maxMoves);
    CloseWindow();
}

void drawGameWindow(Game_context *game)
{
    int screenWidth;
    int screenHeight;
    const char *PLAYER_TURN_LABEL = "Your turn";
    const char *PC_TURN_LABEL = "PC turn";
    int cursorX = 0;
    while (!WindowShouldClose())
    {
        if (game->gameWindow == FINISH)
        {
            // cheese way to show the board after the game is finished (I'm not proud of this)
            WaitTime(1);
            return;
        }
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        updateFont(screenWidth);
        if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && cursorX < game->boardCols - 1)
        {
            cursorX++;
        }
        if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && cursorX > 0)
        {
            cursorX--;
        }
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        if (game->gameState == PLAYER_TURN && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_DOWN)))
        {
            doMove(game, cursorX, PLAYER);
        }
        if (game->gameState == PC_TURN)
        {
            doMove(game, geneticSearch(*game, geneticSearchParameters), PC);
        }
        DrawText(game->gameState == PLAYER_TURN ? PLAYER_TURN_LABEL : PC_TURN_LABEL, screenWidth * 0.02f, screenHeight * 0.08f, getH1Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        drawBoard(game, cursorX);
        EndDrawing();
    }
    CloseWindow();
}

void drawFinishWindow(Game_context *game)
{
    int screenWidth;
    int screenHeight;
    const char *PLAYER_WON_LABEL = game->winner == PLAYER ? "You won!" : game->winner == PC ? "PC won!"
                                                                                            : "Draw!";
    const char *TITLE = "Game finished";
    const char *PRESS_ENTER_LABEL = "Press enter to continue!";
    // Animated gif
    int animFrames = 0;
    ChangeDirectory(GetApplicationDirectory());
    const char *GIF_PATH = game->winner == PLAYER ? "../res/win.gif" : game->winner == PC ? "../res/lost.gif"
                                                                                          : "../res/draw.gif";
    Image gifImage = LoadImageAnim(GIF_PATH, &animFrames);
    Texture2D gifTexture = LoadTextureFromImage(gifImage);
    unsigned int nextFrameDataOffset = 0;
    int currentAnimFrame = 0;
    int frameDelay = 8;
    int frameCounter = 0;
    while (!WindowShouldClose())
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        updateFont(screenWidth);
        if (IsKeyReleased(KEY_ENTER))
        {
            resetGame(game);
            game->gameWindow = WELCOME;
            return;
        }

        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawText(TITLE, screenWidth / 2 - MeasureText(TITLE, getH1Font()) / 2, screenHeight * 0.10f, getH1Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        DrawText(PLAYER_WON_LABEL, screenWidth / 2 - MeasureText(PLAYER_WON_LABEL, getH2Font()) / 2, screenHeight * 0.15f, getH2Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        drawAnimatedGif(&frameCounter, &currentAnimFrame, frameDelay, animFrames, nextFrameDataOffset, gifTexture, gifImage, screenWidth / 2, screenHeight / 2 * 0.55f);
        static int framesCounter = 0;
        framesCounter++;
        if ((framesCounter / 30) % 2)
        {
            DrawText(PRESS_ENTER_LABEL, screenWidth / 2 - MeasureText(PRESS_ENTER_LABEL, getH2Font()) / 2, screenHeight * 0.85f, getH2Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        }
        EndDrawing();
    }
    CloseWindow();
}

void drawBoard(Game_context *game, int cursorX)
{
    const int boardPadding = 100;
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    const int boardWidth = game->boardCols;
    const int boardHeight = game->boardRows;
    // Calculate the board size
    const int maxBoardWidth = screenWidth - boardPadding;
    const int maxBoardHeight = screenHeight - boardPadding;
    const float scaleFactor = fminf(maxBoardWidth / (float)boardWidth, maxBoardHeight / (float)boardHeight);
    const float cellSize = scaleFactor;
    const int boardOffsetX = (screenWidth - (int)(boardWidth * cellSize)) / 2;
    const int boardOffsetY = (screenHeight - (int)(boardHeight * cellSize)) / 2;
    // Draw Board
    DrawRectangleRounded((Rectangle){boardOffsetX, boardOffsetY, boardWidth * cellSize, boardHeight * cellSize}, 0.1f, 20, LIGHTGRAY);
    // Draw cursor
    if (cursorX > -1 && game->gameState == PLAYER_TURN)
        DrawRectangleRounded((Rectangle){boardOffsetX + cellSize * cursorX, boardOffsetY, cellSize, boardHeight * cellSize}, 1, 20, GOLD);
    // Draw circles based on the board state
    for (int row = 0; row < boardHeight; row++)
    {
        for (int col = 0; col < boardWidth; col++)
        {
            const int cellX = boardOffsetX + col * cellSize;
            const int cellY = boardOffsetY + row * cellSize;
            const BoardState cellState = game->board[row][col];
            // Calculate the position and size of the circle
            const float circleSize = cellSize / 2 - 2;
            const float circleX = cellX + cellSize / 2;
            const float circleY = cellY + cellSize / 2;
            switch (cellState)
            {
            case EMPTY:
                DrawCircleV((Vector2){circleX, circleY}, circleSize, WHITE);
                break;
            case PLAYER:
                DrawCircleV((Vector2){circleX, circleY}, circleSize, BLUE);
                break;
            case PC:
                DrawCircleV((Vector2){circleX, circleY}, circleSize, RED);
                break;
            }
        }
    }
}

int drawCoinFlip(Game_context *game)
{
    // choose starting player
    game->startingPlayer = getStartPlayer(game->startingPlayer);
    game->gameState = game->startingPlayer == 1 ? PLAYER_TURN : PC_TURN;

    int screenWidth;
    int screenHeight;

    const int progressBarHeight = 40;
    const int progressMaxValue = 100;
    const int progressSpeed = 2;
    const char *choosingPlayerLabel = "Choosing starting player...";
    const char *playerChoosenLabel = game->startingPlayer == 1 ? "Player starts the game" : "PC starts the game";

    int progressValue = 0;
    // Wait before starting the game
    double waitTime = 2.0; // seconds
    double elapsedTime = 0.0;
    bool progressBarComplete = false;

    // Animated gif
    int animFrames = 0;
    ChangeDirectory(GetApplicationDirectory()); // oh beatiful macOS, gotta love you
    Image gifImage = LoadImageAnim("../res/monkey_angry.gif", &animFrames);
    Texture2D gifTexture = LoadTextureFromImage(gifImage);
    unsigned int nextFrameDataOffset = 0;
    int currentAnimFrame = 0;
    int frameDelay = 8;
    int frameCounter = 0;

    while (!WindowShouldClose())
    {
        frameCounter++;
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        updateFont(screenWidth);
        const int progressBarWidth = (screenWidth * 2) / 3;
        const int progressBarPosX = (screenWidth - progressBarWidth) / 2;
        const int progressBarPosY = (screenHeight - progressBarHeight) / 2;
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        const char *label = progressValue < progressMaxValue ? choosingPlayerLabel : playerChoosenLabel;
        DrawText(label, progressBarPosX, screenHeight * 0.4f, getH1Font(), GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        if (progressValue < progressMaxValue)
            progressValue += progressSpeed;
        else
            progressBarComplete = true;
        GuiProgressBar((Rectangle){progressBarPosX, progressBarPosY, progressBarWidth, progressBarHeight}, NULL, NULL, progressValue, 0, progressMaxValue);
        drawAnimatedGif(&frameCounter, &currentAnimFrame, frameDelay, animFrames, nextFrameDataOffset, gifTexture, gifImage, screenWidth / 2, progressBarPosY + 100);
        EndDrawing();
        if (progressBarComplete)
        {
            elapsedTime += GetFrameTime();
            if (elapsedTime >= waitTime)
            {
                UnloadTexture(gifTexture);
                UnloadImage(gifImage);
                return 0;
            }
        }
    }
    UnloadTexture(gifTexture);
    UnloadImage(gifImage);
    CloseWindow();
    return -1;
}

void drawAnimatedGif(int *frameCounter, int *currentAnimFrame, int frameDelay, int animFrames, unsigned int nextFrameDataOffset, Texture2D gifTexture, Image gifImg, int posX, int posY)
{
    if (memeMode)
    {
        (*frameCounter)++;
        if (*frameCounter >= frameDelay)
        {
            (*currentAnimFrame)++;
            if (*currentAnimFrame >= animFrames)
                *currentAnimFrame = 0;

            nextFrameDataOffset = gifTexture.width * gifTexture.height * 4 * (*currentAnimFrame);
            UpdateTexture(gifTexture, ((unsigned char *)gifImg.data) + nextFrameDataOffset);
            *frameCounter = 0;
        }
        DrawTexture(gifTexture, posX - gifTexture.width / 2, posY, WHITE);
    }
}

void updateStyle(int visualStyleActive, int *prevVisualStyleActive)
{
    if (visualStyleActive != *prevVisualStyleActive)
    {
        ChangeDirectory(GetApplicationDirectory());
        printf("Changing style to %d\n", visualStyleActive);
        switch (visualStyleActive)
        {
        case 0:
            GuiLoadStyleDefault();
            break;
        case 1:
            GuiLoadStyle("../res/jungle.rgs");
            break;
        case 2:
            GuiLoadStyle("../res/lavanda.rgs");
            break;
        case 3:
            GuiLoadStyle("../res/dark.rgs");
            break;
        case 4:
            GuiLoadStyle("../res/bluish.rgs");
            break;
        case 5:
            GuiLoadStyle("../res/cyber.rgs");
            break;
        case 6:
            GuiLoadStyle("../res/terminal.rgs");
            break;
        default:
            break;
        }
        *prevVisualStyleActive = visualStyleActive;
        updateFont(GetScreenWidth()); // some styles change the font, so we need to override it
    }
}

void updateFont(int screenWidth)
{
    if (IsWindowResized())
    {
        const int sm = 800;
        const int md = 1200;
        const int lg = 1900;
        Font font = GuiGetFont();
        if (screenWidth < sm)
        {
            font.baseSize = 10;
            GuiSetFont(font);
        }
        else if (screenWidth < md)
        {
            font.baseSize = 12;
            GuiSetFont(font);
        }
        else if (screenWidth < lg)
        {
            font.baseSize = 16;
            GuiSetFont(font);
        }
        else
        {
            // > large
            font.baseSize = 22;
            GuiSetFont(font);
        }
    }
}

int getH1Font(void)
{
    return GuiGetFont().baseSize * 2;
}

int getH2Font(void)
{
    return GuiGetFont().baseSize * 1.5;
}

int getBodyFont(void)
{
    return GuiGetFont().baseSize;
}
