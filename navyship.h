#pragma once
#include "raylib.h"
#include "common.h"
#include <vector>

class NavyShip {
public:
    NavyShip(Vector2 startPosition);
    void Update();
    void Draw();
    void Shoot();
    void UpdateCannonballs();
    void TakeDamage(float amount);
    void SetSteering(float amount);
    void SetThrottle(float amount);
    Vector2 GetPosition() const { return position; }
    bool IsSinking() const { return isSinking; }
    const std::vector<Cannonball>& GetCannonballs() const { return cannonballs; }

private:
    void DrawShipDetails();
    void UpdatePhysics();
    void UpdateMovement();
    static float lerp(float start, float end, float amount);
    static float Clamp(float value, float min, float max);

    Vector2 position;
    Vector2 velocity{ 0.0f, 0.0f };
    float rotation = PI;
    float steering = 0.0f;
    float throttle = 0.0f;
    float angularVelocity = 0.0f;
    float targetRotation = 0.0f;
    float targetSpeed = 0.0f;
    float health = 100.0f;
    float maxHealth = 100.0f;
    bool isSinking = false;
    float sinkProgress = 0.0f;
    float reloadTime = 0.0f;

    static constexpr float OPTIMAL_RANGE = 300.0f;
    static constexpr float TURN_RATE = 0.1f;
    static constexpr float ACCELERATION = 0.02f;
    static constexpr float ANGULAR_DAMPING = 0.95f;
    static constexpr float LINEAR_DAMPING = 0.98f;
    static constexpr float MAX_SPEED = 0.2f;
    static constexpr float RELOAD_DURATION = 15.0f;

    std::vector<Cannonball> cannonballs;
};
