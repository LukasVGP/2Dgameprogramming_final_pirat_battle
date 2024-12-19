#pragma once
#include "raylib.h"
#include <vector>

class PirateShip {
public:
    struct Cannonball {
        Vector2 pos;
        Vector2 velocity;
        bool active;
    };

    PirateShip();
    void Update();
    void Draw();
    void TakeDamage(float amount);

    // Add these new public member functions
    Vector2 GetPosition() const { return position; }
    const std::vector<Cannonball>& GetCannonballs() const { return cannonballs; }
    bool IsSinking() const { return isSinking; }

    Vector2 position;
    Vector2 targetShipPos;
    std::vector<Cannonball> cannonballs;

private:
#define PI 3.14159265359f
#define MAX_SPEED 2.0f
#define TURN_RATE 2.0f
#define ATTACK_RANGE 400.0f
#define PREFERRED_RANGE 200.0f
#define MAX_SHOOT_RANGE 300.0f
#define ACCURACY 0.8f
#define RELOAD_TIME 30.0f

    Vector2 targetPosition;
    Vector2 velocity;
    float rotation;
    float targetRotation;
    float currentSpeed;
    float health;
    float maxHealth;
    float reloadTimer;
    bool isSinking;
    float sinkProgress;
    bool isInCombat;

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
