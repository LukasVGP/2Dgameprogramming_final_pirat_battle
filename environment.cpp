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
    // Main island shape
    Vector2 center = { 200, 600 };

    // Base island shape (irregular)
    Color sandColor = { 210, 180, 140, 255 };
    DrawCircle(center.x, center.y, 100, sandColor);
    DrawCircle(center.x - 40, center.y + 20, 50, sandColor);
    DrawCircle(center.x + 30, center.y - 30, 60, sandColor);

    // Rocky terrain
    Color rockColor = { 139, 69, 19, 255 };
    DrawCircle(center.x - 20, center.y - 10, 40, rockColor);
    DrawCircle(center.x + 10, center.y + 15, 35, rockColor);

    // Volcano
    DrawTriangle(
        Vector2{ center.x - 40, center.y - 20 },
        Vector2{ center.x + 40, center.y - 20 },
        Vector2{ center.x, center.y - 80 },
        DARKGRAY
    );

    // Volcano crater
    DrawCircle(center.x, center.y - 70, 15, RED);
    DrawCircle(center.x, center.y - 70, 10, ORANGE);

    // Lava streams
    DrawLineEx(
        Vector2{ center.x, center.y - 60 },
        Vector2{ center.x - 20, center.y - 30 },
        3,
        RED
    );
    DrawLineEx(
        Vector2{ center.x, center.y - 60 },
        Vector2{ center.x + 15, center.y - 35 },
        3,
        ORANGE
    );
}

void Environment::DrawNavyIsland() {
    // Main island shape
    Vector2 center = { 1000, 600 };

    // Base island shape (irregular)
    Color grassColor = { 34, 139, 34, 255 };
    DrawCircle(center.x, center.y, 100, DARKGREEN);
    DrawCircle(center.x - 30, center.y - 20, 60, grassColor);
    DrawCircle(center.x + 40, center.y + 30, 50, grassColor);

    // Forest
    for (int i = 0; i < 15; i++) {
        float offsetX = sinf(i * 1.0f) * 60;
        float offsetY = cosf(i * 1.5f) * 60;

        // Tree trunk
        DrawRectangle(
            center.x + offsetX - 5,
            center.y + offsetY - 20,
            10,
            20,
            BROWN
        );

        // Tree foliage
        DrawCircle(
            center.x + offsetX,
            center.y + offsetY - 30,
            15,
            GREEN
        );
        DrawCircle(
            center.x + offsetX - 8,
            center.y + offsetY - 25,
            12,
            GREEN
        );
        DrawCircle(
            center.x + offsetX + 8,
            center.y + offsetY - 25,
            12,
            GREEN
        );
    }

    // Naval fort
    DrawRectangle(center.x - 20, center.y - 40, 40, 30, GRAY);
    DrawRectangle(center.x - 25, center.y - 45, 50, 10, DARKGRAY);
    DrawRectangle(center.x - 15, center.y - 50, 30, 5, LIGHTGRAY);
}
