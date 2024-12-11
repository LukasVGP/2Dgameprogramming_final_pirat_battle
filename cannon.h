#pragma once
#include "raylib.h"

struct Cannonball {
    Vector2 position;
    Vector2 velocity;
    bool active;
};

class Cannon {
public:
    void Fire(Vector2 position, float angle);
    void Update();
    void Draw();

private:
    static const int MAX_BALLS = 20;
    Cannonball balls[MAX_BALLS];
};
