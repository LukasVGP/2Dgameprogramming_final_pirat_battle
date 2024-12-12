#include "menu.h"

Menu::Menu() {
    startButton = { 500, 500, 200, 60 };
    restartButton = { 500, 600, 200, 60 };

    Image pirateImg = LoadImage("resources/pirate_flag.png");
    Image navyImg = LoadImage("resources/navy_flag.png");
    pirateFlag = LoadTextureFromImage(pirateImg);
    navyFlag = LoadTextureFromImage(navyImg);
    UnloadImage(pirateImg);
    UnloadImage(navyImg);
}

void Menu::DrawTitle() {
    const char* title = "PIRATE HUNTER";
    int fontSize = 80;
    int textWidth = MeasureText(title, fontSize);
    DrawText(title, 600 - textWidth / 2, 200, fontSize, GOLD);
}

void Menu::DrawFlags() {
    DrawTexture(pirateFlag, 300, 300, WHITE);
    DrawTexture(navyFlag, 800, 300, WHITE);
}

void Menu::Draw() {
    DrawTitle();
    DrawFlags();

    DrawRectangleRounded(startButton, 0.2f, 8, DARKBLUE);
    DrawText("START", startButton.x + 50, startButton.y + 15, 40, WHITE);
}

void Menu::DrawGameOver(bool playerWon) {
    DrawTitle();
    DrawFlags();

    const char* result = playerWon ? "VICTORY!" : "DEFEAT!";
    DrawText(result, 500, 400, 60, playerWon ? GREEN : RED);

    DrawRectangleRounded(restartButton, 0.2f, 8, DARKBLUE);
    DrawText("RESTART", restartButton.x + 35, restartButton.y + 15, 40, WHITE);
}

bool Menu::IsStartPressed() {
    return CheckCollisionPointRec(GetMousePosition(), startButton) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool Menu::IsRestartPressed() {
    return CheckCollisionPointRec(GetMousePosition(), restartButton) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
