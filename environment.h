#pragma once
#include "raylib.h"

class Environment {
public:
    Environment();
    void Draw();

private:
    void DrawSky();
    void DrawOcean();
    void DrawPirateIsland();
    void DrawNavyIsland();
    void DrawClouds();
    void DrawTowerWithFlag(float x, float y, Color towerColor, Color flagColor);
    void DrawFlagPole(float x, float y);

    struct Cloud {
        Vector2 position;
        float size;
        float speed;
    };

    static const int MAX_CLOUDS = 10;
    Cloud clouds[MAX_CLOUDS];
    float time;
};
