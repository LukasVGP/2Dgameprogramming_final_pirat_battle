#pragma once
#include "raylib.h"
#include <vector>

class PirateShip {
public:
    // Define Cannonball struct first
    struct Cannonball {
        Vector2 pos;
        Vector2 velocity;
        bool active;
    };

    // Constructor and main methods
    PirateShip();
    void Update();
    void Draw();
    void TakeDamage(float amount);
    bool IsPointInCannonRange(Vector2 point);

    // Getter methods for game_logic.cpp
    Vector2 GetPosition() const { return position; }
    const std::vector<Cannonball>& GetCannonballs() const { return cannonballs; }
    bool IsSinking() const { return isSinking; }

    // Constants
    static constexpr float MAX_SPEED = 0.1f;
    static constexpr float TURN_RATE = 2.0f;
    static constexpr float RELOAD_TIME = 2.0f;
    static constexpr float ACCURACY = 0.7f;
    static constexpr float MAX_SHOOT_RANGE = 400.0f;
    static constexpr float ATTACK_RANGE = 300.0f;
    static constexpr float PREFERRED_RANGE = 200.0f;

private:
    // Ship properties
    Vector2 position;
    Vector2 targetPosition;
    Vector2 velocity;
    float rotation;
    float targetRotation;
    float currentSpeed;
    float health;
    float maxHealth;
    bool isSinking;
    float sinkProgress;
    float reloadTimer;

    // Combat properties
    Vector2 targetShipPos;
    bool isInCombat;
    std::vector<Cannonball> cannonballs;

    // Private methods
    void UpdateAI();
    void UpdatePhysics();
    void UpdateCannonballs();
    void DrawShipDetails();
    void AttemptToShoot();
    float GetAngleTo(Vector2 target) const;
    float GetDistanceTo(Vector2 target) const;
    float lerp(float start, float end, float amount);
    float Clamp(float value, float min, float max);
};
