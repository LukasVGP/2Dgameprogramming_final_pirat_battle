#pragma once
#include "raylib.h"
#include <cmath>

class Environment {
public:
    Environment();
    void Draw();

private:
    void DrawOcean();
    void DrawPirateIsland();
    void DrawNavyIsland();
    float time;
};
