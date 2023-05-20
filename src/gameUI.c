#include "includes/gameUI.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include "includes/raygui.h" // TODO: fix include
#pragma GCC diagnostic pop

#undef RAYGUI_IMPLEMENTATION

// Global variables
const bool isDebug = true;

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
    int prevVisualStyleActive = 0;

    // Main game loop
    while (!WindowShouldClose())
    {
        switch (game->gameWindow)
        {
        case WELCOME:
            drawWelcomeWindow(game, &prevVisualStyleActive);
            break;
        case GAME:
            drawGameWindow(game);
            break;
        case FINISH:
            // TODO
            break;
        default:
            break;
        }
    }
}

void drawWelcomeWindow(Game_context *game, int *prevVisualStyleActive)
{
    const char *WELCOME_LABEL = "Welcome to Connect Four!";
    const char *START_BUTTON_LABEL = "Start";
    const char *SETTINGS_TITLE_LABEL = "Settings";
    const char *SELECT_BOARD_SIZE_LABEL = "Select Board Size:";
    const char *SELECT_START_PLAYER_LABEL = "Select Starting Player:";
    const char *CHOOSE_STYLE_LABEL = "Choose Style:";
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    // Define controls variables
    bool isEditingBoardSizeDropdown = false;
    int selectedBoardSize = 0; // 0 = 7x6 (default), 1 = 8x8, 2 = 9x7, 3 = 9x9
    bool isEditingStartPlayerDropdown = false;
    int selectedStartPlayer = 0; // 0 = Random (default), 1 = User, 2 = PC
    // style
    int visualStyleActive = 0;
    while (!WindowShouldClose())
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
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
        DrawText(WELCOME_LABEL, screenWidth * 0.07f, screenHeight * 0.08f, 22, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        GuiLabel((Rectangle){screenWidth * 0.07f, screenHeight * 0.83f, screenWidth * 0.18f, screenHeight * 0.05f}, CHOOSE_STYLE_LABEL);
        if (GuiButton((Rectangle){screenWidth * 0.07f, screenHeight * 0.26f, screenWidth * 0.2f, screenHeight * 0.1f}, START_BUTTON_LABEL))
        {
            game->gameWindow = GAME;
            return;
        }
        visualStyleActive = GuiComboBox((Rectangle){screenWidth * 0.07f, screenHeight * 0.88f, screenWidth * 0.18f, screenHeight * 0.07f}, "Default;Jungle;Lavanda;Dark;Bluish;Cyber;Terminal", visualStyleActive);
        // Settings
        DrawText(SETTINGS_TITLE_LABEL, screenWidth * 0.52f, screenHeight * 0.08f, 18, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        DrawText(SELECT_START_PLAYER_LABEL, screenWidth * 0.52f, screenHeight * 0.41f, GuiGetFont().baseSize, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        if (GuiDropdownBox((Rectangle){screenWidth * 0.52f, screenHeight * 0.44f, screenWidth * 0.18f, screenHeight * 0.07f}, "Random;User;PC", &selectedStartPlayer, isEditingStartPlayerDropdown))
        {
            game->startingPlayer = selectedStartPlayer;
            isEditingStartPlayerDropdown = !isEditingStartPlayerDropdown;
        }
        DrawText(SELECT_BOARD_SIZE_LABEL, screenWidth * 0.52f, screenHeight * 0.21f, GuiGetFont().baseSize, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        if (GuiDropdownBox((Rectangle){screenWidth * 0.52f, screenHeight * 0.24f, screenWidth * 0.18f, screenHeight * 0.07f}, "7x6;8x8;9x7;9x9", &selectedBoardSize, isEditingBoardSizeDropdown))
        {
            int newBoardRows = selectedBoardSize == 0 ? 6 : selectedBoardSize == 1 ? 8
                                                        : selectedBoardSize == 2   ? 7
                                                                                   : 9;
            int newboardCols = selectedBoardSize == 0 ? 7 : selectedBoardSize == 1 ? 8
                                                        : selectedBoardSize == 2   ? 9
                                                                                   : 9;
            changeBoard(game, newBoardRows, newboardCols);
            isEditingBoardSizeDropdown = !isEditingBoardSizeDropdown;
        }
        updateStyle(visualStyleActive, prevVisualStyleActive);
        GuiUnlock();
        EndDrawing();
    }
    CloseWindow();
}

void drawGameWindow(Game_context *game)
{
    // flip coin if starting player is random
    bool exitWindow = false;
    if (game->startingPlayer == 0)
        exitWindow = drawCoinFlip(game);
    if (exitWindow)
        return;
    const char *CURRENT_PLAYER = game->gameState == 1 ? "Your " : "PC ";
    char *currentPlayer;
    asprintf(&currentPlayer, "%s%s", CURRENT_PLAYER, "turn");

    while (!WindowShouldClose())
    {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawText(currentPlayer, screenWidth * 0.02f, screenHeight * 0.08f, 22, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
        drawBoard(game);
        EndDrawing();
    }
    free(currentPlayer);
    CloseWindow();
}

void drawBoard(Game_context *game)
{
    const float targetCellSize = 80.0f;
    const float cornerRadius = 0.1f;

    const int boardWidth = game->boardCols * targetCellSize;
    const int boardHeight = game->boardRows * targetCellSize;
    const int boardPadding = 100;
    const int maxBoardWidth = GetScreenWidth() - boardPadding;
    const int maxBoardHeight = GetScreenHeight() - boardPadding;

    const float scaleFactor = fminf(maxBoardWidth / (float)boardWidth, maxBoardHeight / (float)boardHeight);
    const float cellSize = targetCellSize * scaleFactor;
    const int boardOffsetX = (GetScreenWidth() - (int)(game->boardCols * cellSize)) / 2;
    const int boardOffsetY = (GetScreenHeight() - (int)(game->boardRows * cellSize)) / 2;

    DrawRectangleRounded((Rectangle){boardOffsetX, boardOffsetY, game->boardCols * cellSize, game->boardRows * cellSize}, cornerRadius, 20, LIGHTGRAY);
    for (int row = 0; row < game->boardRows; row++)
    {
        for (int col = 0; col < game->boardCols; col++)
        {
            const int cellX = boardOffsetX + col * cellSize;
            const int cellY = boardOffsetY + row * cellSize;
            const boardState cellState = game->board[row][col];
            switch (cellState)
            {
            case EMPTY:
                DrawCircle(cellX + cellSize / 2, cellY + cellSize / 2, cellSize / 2 - 2, WHITE);
                break;
            case PLAYER:
                DrawCircle(cellX + cellSize / 2, cellY + cellSize / 2, cellSize / 2 - 2, BLUE);
                break;
            case PC:
                DrawCircle(cellX + cellSize / 2, cellY + cellSize / 2, cellSize / 2 - 2, RED);
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

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    const int progressBarWidth = (screenWidth * 2) / 3;
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
        const int progressBarPosX = (screenWidth - progressBarWidth) / 2;
        const int progressBarPosY = (screenHeight - progressBarHeight) / 2;
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        const char *label = progressValue < progressMaxValue ? choosingPlayerLabel : playerChoosenLabel;
        DrawText(label, screenWidth / 2 - MeasureText(label, GuiGetFont().baseSize), screenHeight * 0.4f, 22, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
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

void updateStyle(int visualStyleActive, int *prevVisualStyleActive)
{
    if (visualStyleActive != *prevVisualStyleActive)
    {
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
    }
}
