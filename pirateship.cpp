#include "pirateship.h"
#include <cmath>
#include <algorithm>

PirateShip::PirateShip() {
    position = ISLAND_CENTER;
}

void PirateShip::Update() {
    if (isSinking) {
        sinkProgress += 0.01f;
        if (sinkProgress >= 1.0f) return;
    }

    UpdateAIMovement();
    UpdatePhysics();
    UpdateCannonballs();
}

void PirateShip::UpdateAIMovement() {
    circleTime += GetFrameTime() * CIRCLE_SPEED;

    // Calculate next position on circle
    float nextCircleTime = circleTime + GetFrameTime() * CIRCLE_SPEED;
    Vector2 nextPosition;
    nextPosition.x = ISLAND_CENTER.x + cosf(nextCircleTime) * CIRCLE_RADIUS;
    nextPosition.y = ISLAND_CENTER.y + sinf(nextCircleTime) * CIRCLE_RADIUS;

    // Calculate direction to face movement
    float dx = nextPosition.x - position.x;
    float dy = nextPosition.y - position.y;
    targetRotation = atan2f(dy, dx);

    // Set constant speed for smooth movement
    targetSpeed = MAX_SPEED * 0.5f; // Half max speed for steady circular motion

    // Update position directly for perfect circle
    position.x = ISLAND_CENTER.x + cosf(circleTime) * CIRCLE_RADIUS;
    position.y = ISLAND_CENTER.y + sinf(circleTime) * CIRCLE_RADIUS;
}

void PirateShip::UpdatePhysics() {
    float deltaTime = GetFrameTime();

    // Angular physics
    float rotationDiff = targetRotation - rotation;
    while (rotationDiff > PI) rotationDiff -= 2 * PI;
    while (rotationDiff < -PI) rotationDiff += 2 * PI;

    float targetAngularVel = rotationDiff * TURN_RATE;
    angularVelocity = lerp(angularVelocity, targetAngularVel, ANGULAR_DAMPING * deltaTime);
    rotation += angularVelocity * deltaTime;

    // Linear physics
    float targetVelocityMagnitude = targetSpeed * MAX_SPEED;
    Vector2 targetVelocity = {
        cosf(rotation) * targetVelocityMagnitude,
        sinf(rotation) * targetVelocityMagnitude
    };

    velocity.x = lerp(velocity.x, targetVelocity.x, ACCELERATION * deltaTime);
    velocity.y = lerp(velocity.y, targetVelocity.y, ACCELERATION * deltaTime);

    velocity.x *= LINEAR_DAMPING;
    velocity.y *= LINEAR_DAMPING;

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void PirateShip::Draw() {
    Color mainColor = isSinking ?
        ColorAlpha(MAROON, 1.0f - sinkProgress) : MAROON;

    Vector2 center = position;
    float rot = rotation * RAD2DEG;

    // Enhanced hull
    DrawRectanglePro(
        Rectangle{ center.x, center.y, 50, 25 },
        Vector2{ 25, 12.5f },
        rot,
        mainColor
    );

    // Deck details
    Vector2 rotatedOffset;
    rotatedOffset.x = cosf(rotation);
    rotatedOffset.y = sinf(rotation);

    // Cannon ports
    for (int i = -1; i <= 1; i++) {
        Vector2 portPos = {
            center.x + rotatedOffset.y * i * 8,
            center.y - rotatedOffset.x * i * 8
        };
        DrawCircle(portPos.x, portPos.y, 2, BLACK);
    }

    // Main mast
    DrawLineEx(
        Vector2{ center.x, center.y },
        Vector2{ center.x, center.y - 35 },
        3.0f,
        DARKBROWN
    );

    // Pirate sail with skull
    DrawTriangle(
        Vector2{ center.x, center.y - 35 },
        Vector2{ center.x - 20, center.y },
        Vector2{ center.x + 20, center.y },
        BLACK
    );

    // Skull emblem
    Vector2 skullPos = {
        center.x,
        center.y - 20
    };
    DrawCircle(skullPos.x, skullPos.y, 5, WHITE);
    DrawCircle(skullPos.x - 2, skullPos.y - 1, 1, BLACK);
    DrawCircle(skullPos.x + 2, skullPos.y - 1, 1, BLACK);
    DrawLine(skullPos.x - 3, skullPos.y + 2, skullPos.x + 3, skullPos.y + 2, BLACK);

    DrawShipDetails();
}


void PirateShip::DrawShipDetails() {
    Vector2 center = position;

    // Health bar
    float healthBarWidth = 40;
    float healthBarHeight = 5;
    float healthPercentage = health / maxHealth;

    DrawRectangle(
        static_cast<int>(center.x - healthBarWidth / 2),
        static_cast<int>(center.y - 40),
        static_cast<int>(healthBarWidth),
        static_cast<int>(healthBarHeight),
        RED
    );

    DrawRectangle(
        static_cast<int>(center.x - healthBarWidth / 2),
        static_cast<int>(center.y - 40),
        static_cast<int>(healthBarWidth * healthPercentage),
        static_cast<int>(healthBarHeight),
        GREEN
    );

    // Ship name
    const char* name = "Pirate";
    DrawText(name,
        static_cast<int>(center.x - MeasureText(name, 20) / 2),
        static_cast<int>(center.y - 30),
        20,
        WHITE);

    // Reload indicator
    if (reloadTime > 0.0f) {
        char reloadText[32];
        sprintf_s(reloadText, sizeof(reloadText), "Reload: %.1f", reloadTime);
        DrawText(reloadText,
            static_cast<int>(center.x - MeasureText(reloadText, 20) / 2),
            static_cast<int>(center.y - 50),
            20,
            YELLOW);
    }

    // Draw active cannonballs
    for (const auto& ball : cannonballs) {
        if (ball.active) {
            DrawCircle(
                static_cast<int>(ball.pos.x),
                static_cast<int>(ball.pos.y),
                3,
                BLACK);
        }
    }
}

void PirateShip::Shoot() {
    if (reloadTime <= 0.0f) {
        // Left broadside
        for (int i = 0; i < 3; i++) {
            Cannonball ball;
            ball.pos = position;
            float angle = rotation - PI / 2;
            ball.velocity = { cosf(angle) * 5.0f, sinf(angle) * 5.0f };
            ball.active = true;
            ball.distanceTraveled = 0.0f;
            cannonballs.push_back(ball);
        }

        // Right broadside
        for (int i = 0; i < 3; i++) {
            Cannonball ball;
            ball.pos = position;
            float angle = rotation + PI / 2;
            ball.velocity = { cosf(angle) * 5.0f, sinf(angle) * 5.0f };
            ball.active = true;
            ball.distanceTraveled = 0.0f;
            cannonballs.push_back(ball);
        }

        reloadTime = RELOAD_DURATION;
    }
}

void PirateShip::UpdateCannonballs() {
    if (reloadTime > 0.0f) {
        reloadTime -= GetFrameTime();
    }

    for (auto& ball : cannonballs) {
        if (ball.active) {
            Vector2 oldPos = ball.pos;
            ball.pos.x += ball.velocity.x;
            ball.pos.y += ball.velocity.y;

            float dx = ball.pos.x - oldPos.x;
            float dy = ball.pos.y - oldPos.y;
            ball.distanceTraveled += sqrtf(dx * dx + dy * dy);

            if (ball.distanceTraveled > MAX_SHOOT_RANGE ||
                ball.pos.x < 0 || ball.pos.x > 1200 ||
                ball.pos.y < 0 || ball.pos.y > 1200) {
                ball.active = false;
            }
        }
    }

    cannonballs.erase(
        std::remove_if(cannonballs.begin(), cannonballs.end(),
            [](const Cannonball& ball) { return !ball.active; }),
        cannonballs.end());
}

void PirateShip::TakeDamage(float amount) {
    health -= amount;
    if (health <= 0 && !isSinking) {
        health = 0;
        isSinking = true;
        sinkProgress = 0.0f;
    }
}

float PirateShip::lerp(float start, float end, float amount) {
    return start + (end - start) * amount;
}

float PirateShip::Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
