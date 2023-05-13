#include "includes/gameUI.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "includes/raygui.h"

const bool isDebug = true;
bool startGame = false;
int boardSize = 0;      // 0: 7x6, 1: 8x8, 2: 9x7, 3: 9x9

void initUI(int screenWidth, int screenHeight) {
    InitWindow(screenWidth, screenHeight, "Connect Four");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetWindowState(FLAG_WINDOW_RESIZABLE);
}

void loop(Game_context game)
{
    // Const text
    const char *WelcomeLabel = "WELCOME TO CONNECT FOUR!";
    const char *StartButtonText = "START";
    // Settings
    const char *SettingsTitleLabel = "Settings";
    const char *SelectBoardSizeLabel = "Select Board Size:";
    const char *DropdownBoardSizes = "7x6;8x8;9x7;9x9";

    const char *SelectStartPlayerLabel = "Select Starting Player:";
    const char *DropdownStartPlayers = "Random;User;PC";

    // Define controls variables
    bool isEditingBoardSizeDropdown = false;
    int BoardSizeDropdownIsActive = 0;
    bool isEditingStartPlayerDropdown = false;
    int StartPlayerDropdownIsActive = 0;

    // Main game loop
    while (!WindowShouldClose())
    {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        BeginDrawing();
        if (isDebug)
        {
            DrawFPS(screenWidth - 80, 0);
        }
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        if (startGame)
        {
            // display selected Sele
        }
        else
        {
            if (isEditingBoardSizeDropdown || isEditingStartPlayerDropdown)
            {
                GuiLock();
            }
            GuiLabel((Rectangle){screenWidth * 0.07f, screenHeight * 0.08f, screenWidth * 0.24f, screenHeight * 0.05f}, WelcomeLabel);
            GuiLabel((Rectangle){screenWidth * 0.52f, screenHeight * 0.08f, screenWidth * 0.18f, screenHeight * 0.05f}, SettingsTitleLabel);
            GuiLabel((Rectangle){screenWidth * 0.52f, screenHeight * 0.2f, screenWidth * 0.18f, screenHeight * 0.05f}, SelectBoardSizeLabel);
            GuiLabel((Rectangle){screenWidth * 0.52f, screenHeight * 0.58f, screenWidth * 0.18f, screenHeight * 0.05f}, SelectStartPlayerLabel);
            if (GuiButton((Rectangle){screenWidth * 0.07f, screenHeight * 0.36f, screenWidth * 0.2f, screenHeight * 0.1f}, StartButtonText))
            {
                startGame = true;
            }

            if (GuiDropdownBox((Rectangle){screenWidth * 0.52f, screenHeight * 0.24f, screenWidth * 0.18f, screenHeight * 0.07f},
                               DropdownBoardSizes, &BoardSizeDropdownIsActive, isEditingBoardSizeDropdown))
            {
                isEditingBoardSizeDropdown = !isEditingBoardSizeDropdown;
            }
            if (GuiDropdownBox((Rectangle){screenWidth * 0.52f, screenHeight * 0.64f, screenWidth * 0.18f, screenHeight * 0.07f},
                               DropdownStartPlayers, &StartPlayerDropdownIsActive, isEditingStartPlayerDropdown))
            {
                isEditingStartPlayerDropdown = !isEditingStartPlayerDropdown;
            }
            GuiUnlock();
        }

        EndDrawing();
    }
    CloseWindow();
}