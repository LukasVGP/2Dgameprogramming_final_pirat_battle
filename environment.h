#pragma once
#include "raylib.h"
#include <cmath>

class Environment {
public:
    Environment();
    void Draw();

private:
    void DrawSky();
    void DrawOcean();
    void DrawPirateIsland();
    void DrawNavyIsland();
    void DrawWaves();
    void DrawClouds();

    struct Cloud {
        Vector2 position;
        float size;
        float speed;
    };

    static const int MAX_CLOUDS = 10;
    Cloud clouds[MAX_CLOUDS];
    float time;
};
