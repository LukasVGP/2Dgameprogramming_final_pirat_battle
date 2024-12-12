#pragma once
#include "raylib.h"

class Menu {
public:
    Menu();
    void Draw();
    bool IsStartPressed();
    void DrawGameOver(bool playerWon);
    bool IsRestartPressed();

private:
    Rectangle startButton;
    Rectangle restartButton;
    Texture2D pirateFlag;
    Texture2D navyFlag;
    void DrawTitle();
    void DrawFlags();
};
