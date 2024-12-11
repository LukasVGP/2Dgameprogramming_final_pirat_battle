#pragma once
#include "raylib.h"
#include "common.h"
#include <vector>
#include <string>

class PirateShip {
public:
    PirateShip();
    void Update();
    void Draw();
    void Shoot();
    void UpdateCannonballs();
    void TakeDamage(float amount);
    Vector2 GetPosition() const { return position; }
    bool IsSinking() const { return isSinking; }
    const std::vector<Cannonball>& GetCannonballs() const { return cannonballs; }

private:
    void DrawShipDetails();
    void UpdatePhysics();
    void UpdateAIMovement();
    static float lerp(float start, float end, float amount);
    static float Clamp(float value, float min, float max);

    Vector2 position{ 200.0f, 600.0f };
    Vector2 velocity{ 0.0f, 0.0f };
    float rotation = 0.0f;
    float angularVelocity = 0.0f;
    float targetRotation = 0.0f;
    float targetSpeed = 0.0f;
    float circleTime = 0.0f;
    float health = 100.0f;
    float maxHealth = 100.0f;
    bool isSinking = false;
    float sinkProgress = 0.0f;
    float reloadTime = 0.0f;

    static constexpr float CIRCLE_RADIUS = 150.0f;
    static constexpr float CIRCLE_SPEED = 0.1f;
    static constexpr Vector2 ISLAND_CENTER = { 200.0f, 600.0f };
    static constexpr float TURN_RATE = 0.1f;
    static constexpr float ACCELERATION = 0.02f;
    static constexpr float ANGULAR_DAMPING = 0.95f;
    static constexpr float LINEAR_DAMPING = 0.98f;
    static constexpr float MAX_SPEED = 0.2f;
    static constexpr float RELOAD_DURATION = 15.0f;
    static constexpr float MAX_SHOOT_RANGE = 80.0f;

    std::vector<Cannonball> cannonballs;
};
