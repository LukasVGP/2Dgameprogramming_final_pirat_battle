#include "environment.h"

Environment::Environment() {
    // Initialize clouds with slower speed
    for (int i = 0; i < MAX_CLOUDS; i++) {
        clouds[i].position = { (float)GetRandomValue(0, 1200), (float)GetRandomValue(50, 200) };
        clouds[i].size = GetRandomValue(30, 60);
        // Reduce speed to 10% by dividing by 300 instead of 30
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

void Environment::DrawClouds() {
    for (int i = 0; i < MAX_CLOUDS; i++) {
        clouds[i].position.x += clouds[i].speed;
        if (clouds[i].position.x > 1200) clouds[i].position.x = -clouds[i].size;
        DrawCircle(clouds[i].position.x, clouds[i].position.y, clouds[i].size, WHITE);
        DrawCircle(clouds[i].position.x + clouds[i].size * 0.5f,
            clouds[i].position.y, clouds[i].size * 0.8f, WHITE);
    }
}
