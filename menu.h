#pragma once
#include "raylib.h"
#include <cmath>
#include <cstring>

class Menu {
public:
    Menu();
    ~Menu();
    void Draw();
    bool IsStartPressed();
    void DrawGameOver(bool playerWon);
    bool IsRestartPressed();

private:
    static const int FLAG_WIDTH = 150;
    static const int FLAG_HEIGHT = 100;
    static const int FLAG_SPACING = 50;

    Rectangle startButton;
    Rectangle restartButton;
    float waveOffset;
    Vector2 flagSize;

    void DrawTitle();
    void DrawFlags();
    void DrawWaves();
    void DrawPirateFlag(float x, float y);
    void DrawUnionJack(float x, float y);
};
