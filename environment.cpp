#include "environment.h"
#include <cmath>

Environment::Environment() {
    for (int i = 0; i < MAX_CLOUDS; i++) {
        clouds[i].position = { static_cast<float>(GetRandomValue(0, 1200)),
                             static_cast<float>(GetRandomValue(50, 200)) };
        clouds[i].size = static_cast<float>(GetRandomValue(30, 60));
        clouds[i].speed = GetRandomValue(1, 3) / 300.0f;
    }
    time = 0;
}

void Environment::Draw() {
    DrawSky();
    DrawOcean();
    DrawPirateIsland();
    DrawNavyIsland();
    DrawClouds();
    time += GetFrameTime();
}

void Environment::DrawSky() {
    Color skyTop = { 135, 206, 235, 255 };
    Color skyBottom = { 173, 216, 230, 255 };
    DrawRectangleGradientV(0, 0, 1200, 400, skyTop, skyBottom);
    DrawCircle(1000, 100, 40, YELLOW);
    DrawCircle(1000, 100, 35, GOLD);
}

void Environment::DrawOcean() {
    for (int y = 400; y < 1200; y += 20) {
        for (int x = 0; x < 1200; x += 40) {
            float wave = sinf((x + time * 50) / 100.0f) * 5;
            DrawLineEx(
                { static_cast<float>(x), static_cast<float>(y) + wave },
                { static_cast<float>(x + 40),
                  static_cast<float>(y) + sinf((x + 40 + time * 50) / 100.0f) * 5 },
                2,
                ColorAlpha(DARKBLUE, 0.3f)
            );
        }
    }
}

void Environment::DrawPirateIsland() {
    DrawCircle(200, 600, 90, BROWN);
    DrawCircle(180, 580, 40, BROWN);
    DrawCircle(230, 620, 45, BROWN);

    Color lavaColor = { 255, 69, 0, 255 };
    DrawTriangle(
        Vector2{ 160, 600 },
        Vector2{ 240, 600 },
        Vector2{ 200, 500 },
        DARKGRAY
    );
    DrawCircle(200, 500, 20, lavaColor);

    for (int i = 0; i < 5; i++) {
        float offset = sinf(time * 2 + i) * 5;
        DrawCircle(200 + offset, 470 - i * 10, 10 - i, LIGHTGRAY);
    }

    DrawLineEx(Vector2{ 200, 485 }, Vector2{ 170, 550 }, 4, lavaColor);
    DrawLineEx(Vector2{ 200, 485 }, Vector2{ 230, 560 }, 4, lavaColor);
    DrawLineEx(Vector2{ 200, 485 }, Vector2{ 180, 570 }, 4, lavaColor);

    for (int i = 0; i < 15; i++) {
        float x = 150 + GetRandomValue(-20, 20) + (i * 10);
        float y = 620 + GetRandomValue(-10, 10);
        DrawTriangle(
            Vector2{ x - 8, y + 10 },
            Vector2{ x + 8, y + 10 },
            Vector2{ x, y - 15 },
            DARKGREEN
        );
        DrawTriangle(
            Vector2{ x - 6, y + 5 },
            Vector2{ x + 6, y + 5 },
            Vector2{ x, y - 20 },
            GREEN
        );
    }

    DrawRectangle(160, 540, 80, 30, DARKBROWN);
    DrawRectangle(180, 510, 40, 30, DARKGRAY);
    DrawRectangle(185, 480, 30, 30, BLACK);

    DrawTowerWithFlag(150, 530, DARKBROWN, RED);
    DrawTowerWithFlag(250, 530, DARKBROWN, RED);
}

void Environment::DrawNavyIsland() {
    DrawCircle(1000, 600, 100, GREEN);
    DrawCircle(960, 580, 50, GREEN);
    DrawCircle(1040, 620, 55, GREEN);

    DrawTriangle(
        Vector2{ 940, 600 },
        Vector2{ 980, 520 },
        Vector2{ 1020, 600 },
        GRAY
    );
    DrawTriangle(
        Vector2{ 980, 600 },
        Vector2{ 1020, 530 },
        Vector2{ 1060, 600 },
        DARKGRAY
    );
    DrawTriangle(
        Vector2{ 960, 600 },
        Vector2{ 1000, 540 },
        Vector2{ 1040, 600 },
        LIGHTGRAY
    );

    DrawTriangle(
        Vector2{ 980, 530 },
        Vector2{ 1000, 520 },
        Vector2{ 1020, 530 },
        WHITE
    );

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float x = 950 + i * 30;
            float y = 570 + j * 15;
            DrawTriangle(
                Vector2{ x - 6, y + 15 },
                Vector2{ x + 6, y + 15 },
                Vector2{ x, y },
                DARKGREEN
            );
            DrawTriangle(
                Vector2{ x - 4, y + 8 },
                Vector2{ x + 4, y + 8 },
                Vector2{ x, y - 7 },
                GREEN
            );
        }
    }

    DrawRectangle(960, 540, 100, 40, DARKBLUE);
    DrawRectangle(980, 510, 60, 30, DARKBLUE);
    DrawRectangle(985, 480, 50, 30, DARKBLUE);

    DrawFlagPole(970, 480);
    DrawFlagPole(1050, 480);

    DrawRectangle(920, 610, 160, 15, BROWN);
    for (int i = 0; i < 5; i++) {
        DrawRectangle(920 + i * 40, 610, 5, 25, BROWN);
    }
}

void Environment::DrawClouds() {
    for (int i = 0; i < MAX_CLOUDS; i++) {
        clouds[i].position.x += clouds[i].speed;
        if (clouds[i].position.x > 1200) clouds[i].position.x = -clouds[i].size;
        DrawCircle(static_cast<int>(clouds[i].position.x),
            static_cast<int>(clouds[i].position.y),
            clouds[i].size, WHITE);
        DrawCircle(static_cast<int>(clouds[i].position.x + clouds[i].size * 0.5f),
            static_cast<int>(clouds[i].position.y),
            clouds[i].size * 0.8f, WHITE);
    }
}

void Environment::DrawTowerWithFlag(float x, float y, Color towerColor, Color flagColor) {
    DrawRectangle(static_cast<int>(x), static_cast<int>(y), 20, 40, towerColor);
    DrawTriangle(
        Vector2{ x + 10, y - 20 },
        Vector2{ x + 30, y },
        Vector2{ x + 10, y },
        flagColor
    );
}

void Environment::DrawFlagPole(float x, float y) {
    DrawRectangle(static_cast<int>(x), static_cast<int>(y), 5, 40, DARKGRAY);
    DrawRectangle(static_cast<int>(x - 10), static_cast<int>(y), 25, 10, WHITE);
}
