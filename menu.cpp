#include "menu.h"

Menu::Menu() {
    startButton = { 500, 500, 200, 60 };
    restartButton = { 500, 600, 200, 60 };
    waveOffset = 0.0f;
    flagSize = { FLAG_WIDTH, FLAG_HEIGHT };
}

Menu::~Menu() {}

void Menu::DrawWaves() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKBLUE);

    waveOffset += 0.5f;
    for (int y = 0; y < GetScreenHeight(); y += 100) {
        for (int x = -100; x < GetScreenWidth() + 100; x += 100) {
            float yPos = y + sinf((x + waveOffset) * 0.05f) * 20;
            DrawCircle(x, yPos, 50, ColorAlpha(BLUE, 0.3f));
        }
    }
}

void Menu::DrawPirateFlag(float x, float y) {
    DrawRectangle(x, y, flagSize.x, flagSize.y, BLACK);

    Vector2 center = { x + flagSize.x / 2, y + flagSize.y / 2 };
    DrawCircle(center.x, center.y - 10, 20, WHITE);
    DrawLineEx({ x + 20, y + flagSize.y - 20 },
        { x + flagSize.x - 20, y + flagSize.y - 20 },
        5, WHITE);
}

void Menu::DrawUnionJack(float x, float y) {
    DrawRectangle(x, y, flagSize.x, flagSize.y, DARKBLUE);

    DrawRectangle(x + flagSize.x / 2 - 5, y, 10, flagSize.y, WHITE);
    DrawRectangle(x, y + flagSize.y / 2 - 5, flagSize.x, 10, WHITE);
    DrawLineEx({ x, y }, { x + flagSize.x, y + flagSize.y }, 8, RED);
    DrawLineEx({ x + flagSize.x, y }, { x, y + flagSize.y }, 8, RED);
}

void Menu::DrawTitle() {
    const char* title = "PIRATE HUNTER";
    int fontSize = 80;
    int textWidth = MeasureText(title, fontSize);
    int screenWidth = GetScreenWidth();
    int centerX = screenWidth / 2;

    DrawText(title, centerX - textWidth / 2 + 4, 204, fontSize, BLACK);

    for (int i = 0; i < strlen(title); i++) {
        char c[2] = { title[i], '\0' };
        Color letterColor = ColorFromHSV(static_cast<float>(i * 15), 0.7f, 1.0f);
        Vector2 position = {
            static_cast<float>(centerX - textWidth / 2 + MeasureText(title, fontSize) / strlen(title) * i),
            200.0f + sinf(GetTime() * 2 + i) * 5
        };
        DrawTextEx(GetFontDefault(), c, position, fontSize, 2, letterColor);
    }
}

void Menu::DrawFlags() {
    int screenWidth = GetScreenWidth();
    int centerX = screenWidth / 2;
    int titleWidth = MeasureText("PIRATE HUNTER", 80);

    DrawPirateFlag(centerX - (titleWidth / 2) - FLAG_WIDTH - FLAG_SPACING, 180);
    DrawUnionJack(centerX + (titleWidth / 2) + FLAG_SPACING, 180);
}

void Menu::Draw() {
    DrawWaves();
    DrawTitle();
    DrawFlags();

    DrawRectangleRounded(startButton, 0.2f, 8, ColorAlpha(DARKBLUE, 0.8f));
    DrawRectangleRoundedLines(startButton, 0.2f, 8, GOLD);
    DrawText("START", startButton.x + 50, startButton.y + 15, 40, WHITE);
}

void Menu::DrawGameOver(bool playerWon) {
    DrawWaves();
    DrawTitle();
    DrawFlags();

    const char* result = playerWon ? "VICTORY!" : "DEFEAT!";
    int resultWidth = MeasureText(result, 60);
    DrawText(result, GetScreenWidth() / 2 - resultWidth / 2, 400, 60, playerWon ? GREEN : RED);

    DrawRectangleRounded(restartButton, 0.2f, 8, ColorAlpha(DARKBLUE, 0.8f));
    DrawRectangleRoundedLines(restartButton, 0.2f, 8, GOLD);
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
