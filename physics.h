#pragma once
#include "raylib.h"
#include "navyship.h"
#include "pirateship.h"

class Physics {
public:
    static bool CheckCollision(const NavyShip& ship1, const PirateShip& ship2);
    static bool CheckCannonHit(const Cannonball& ball, const NavyShip& ship);
    static bool CheckCannonHit(const Cannonball& ball, const PirateShip& ship);
    static Vector2 CalculateWaveEffect(Vector2 position, float time);

private:
    static float CalculateDistance(Vector2 pos1, Vector2 pos2);
    static constexpr float COLLISION_RADIUS = 25.0f;
    static constexpr float CANNONBALL_RADIUS = 3.0f;
};
