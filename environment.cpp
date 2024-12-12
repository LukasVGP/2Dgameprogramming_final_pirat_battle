#include "environment.h"

Environment::Environment() {
    time = 0;
}

void Environment::Draw() {
    DrawOcean();
    DrawPirateIsland();
    DrawNavyIsland();
    time += GetFrameTime();
}

void Environment::DrawOcean() {
    // Fill entire background with ocean
    for (int y = 0; y < 1200; y += 20) {
        for (int x = 0; x < 1200; x += 40) {
            float wave = sinf((x + time * 50) / 100.0f) * 5;
            DrawLineEx(
                { (float)x, (float)y + wave },
                { (float)x + 40, (float)y + sinf((x + 40 + time * 50) / 100.0f) * 5 },
                2,
                ColorAlpha(DARKBLUE, 0.3f)
            );
        }
    }
}

void Environment::DrawPirateIsland() {
    DrawCircle(200, 600, 80, BROWN);
    DrawRectangle(160, 540, 80, 20, DARKBROWN);
    DrawRectangle(180, 520, 20, 40, DARKGRAY);
    DrawRectangle(185, 480, 30, 20, BLACK);
    DrawCircle(200, 490, 5, WHITE);
    DrawLine(195, 495, 205, 495, WHITE);
}

void Environment::DrawNavyIsland() {
    DrawCircle(1000, 600, 80, GREEN);
    DrawRectangle(960, 540, 100, 30, DARKBLUE);
    DrawRectangle(980, 520, 20, 40, GRAY);
    DrawRectangle(985, 480, 30, 20, DARKBLUE);
    DrawRectangle(995, 480, 10, 20, RED);
    DrawRectangle(985, 485, 30, 10, RED);
}
