#pragma once
#include "raylib.h"

struct Cannonball {
    Vector2 pos;
    Vector2 velocity;
    float rotation;
    bool active;
    float distanceTraveled;
    float lifetime = 0.0f;
    float maxLifetime = 2.0f;
};
