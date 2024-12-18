#include "pirateship.h"
#include <cmath>
#include <algorithm>

PirateShip::PirateShip() {
    position.x = ISLAND_CENTER.x + CIRCLE_RADIUS;
    position.y = ISLAND_CENTER.y;
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
    Vector2 nextPos;
    nextPos.x = ISLAND_CENTER.x + cosf(circleTime + 0.1f) * CIRCLE_RADIUS;
    nextPos.y = ISLAND_CENTER.y + sinf(circleTime + 0.1f) * CIRCLE_RADIUS;

    // Calculate current target position
    Vector2 targetPos;
    targetPos.x = ISLAND_CENTER.x + cosf(circleTime) * CIRCLE_RADIUS;
    targetPos.y = ISLAND_CENTER.y + sinf(circleTime) * CIRCLE_RADIUS;

    // Calculate direction to next position for rotation
    float dx = nextPos.x - position.x;
    float dy = nextPos.y - position.y;
    targetRotation = atan2f(dy, dx);  // Direct angle calculation

    // Set constant speed for continuous movement
    targetSpeed = MAX_SPEED * 0.5f;

    // Move towards target position
    position = targetPos;
}

void PirateShip::UpdatePhysics() {
    float deltaTime = GetFrameTime();

    // Update rotation
    float rotationDiff = targetRotation - rotation;
    while (rotationDiff > PI) rotationDiff -= 2 * PI;
    while (rotationDiff < -PI) rotationDiff += 2 * PI;
    float targetAngularVel = rotationDiff * TURN_RATE;
    angularVelocity = lerp(angularVelocity, targetAngularVel, ANGULAR_DAMPING * deltaTime);
    rotation += angularVelocity * deltaTime;

    // Calculate forward direction based on rotation
    Vector2 forwardDir = {
        cosf(rotation),
        sinf(rotation)
    };

    // Apply velocity in the forward direction
    float targetVelocityMagnitude = targetSpeed * MAX_SPEED;
    Vector2 targetVelocity = {
        forwardDir.x * targetVelocityMagnitude,
        forwardDir.y * targetVelocityMagnitude
    };

    // Smoothly interpolate current velocity towards target velocity
    velocity.x = lerp(velocity.x, targetVelocity.x, ACCELERATION * deltaTime);
    velocity.y = lerp(velocity.y, targetVelocity.y, ACCELERATION * deltaTime);

    // Apply damping
    velocity.x *= LINEAR_DAMPING;
    velocity.y *= LINEAR_DAMPING;
}

void PirateShip::Draw() {
    Color mainColor = isSinking ? ColorAlpha(MAROON, 1.0f - sinkProgress) : MAROON;
    Vector2 center = position;
    float rot = rotation * RAD2DEG;
    Vector2 rotatedOffset = { cosf(rotation), sinf(rotation) };

    // Main hull
    DrawRectanglePro(
        Rectangle{ center.x, center.y, 70, 25 },
        Vector2{ 35, 12.5f },
        rot,
        mainColor
    );

    // Sharp bow with decorative figurehead
    Vector2 bowTip = {
        center.x + rotatedOffset.x * 40,
        center.y + rotatedOffset.y * 40
    };
    DrawTriangle(
        Vector2{ center.x + rotatedOffset.x * 35, center.y + rotatedOffset.y * 35 - 12 },
        Vector2{ center.x + rotatedOffset.x * 35, center.y + rotatedOffset.y * 35 + 12 },
        bowTip,
        DARKBROWN
    );

    // Ornate stern castle
    DrawRectanglePro(
        Rectangle{ center.x - rotatedOffset.x * 30, center.y - rotatedOffset.y * 30, 30, 35 },
        Vector2{ 15, 17.5f },
        rot,
        DARKBROWN
    );

    // Two tall masts with ragged sails
    for (int i = -1; i <= 1; i += 2) {
        float mastOffset = i * 20;
        DrawLineEx(
            Vector2{ center.x + rotatedOffset.x * mastOffset, center.y + rotatedOffset.y * mastOffset },
            Vector2{ center.x + rotatedOffset.x * mastOffset, center.y + rotatedOffset.y * mastOffset - 45 },
            4.0f,
            DARKBROWN
        );

        // Triangular pirate sails
        DrawTriangle(
            Vector2{ center.x + rotatedOffset.x * mastOffset, center.y + rotatedOffset.y * mastOffset - 45 },
            Vector2{ center.x + rotatedOffset.x * (mastOffset - 25), center.y + rotatedOffset.y * mastOffset - 10 },
            Vector2{ center.x + rotatedOffset.x * (mastOffset + 25), center.y + rotatedOffset.y * mastOffset - 10 },
            BLACK
        );
    }

    // Jolly Roger flag
    DrawRectanglePro(
        Rectangle{ center.x, center.y - 50, 25, 20 },
        Vector2{ 12.5f, 0 },
        rot,
        BLACK
    );

    // Skull on flag
    Vector2 skullPos = {
        center.x,
        center.y - 40
    };
    DrawCircle(skullPos.x, skullPos.y, 6, WHITE);
    DrawCircle(skullPos.x - 2, skullPos.y - 1, 2, BLACK);
    DrawCircle(skullPos.x + 2, skullPos.y - 1, 2, BLACK);
    DrawLine(skullPos.x - 3, skullPos.y + 2, skullPos.x + 3, skullPos.y + 2, BLACK);

    DrawShipDetails();
}

void PirateShip::DrawShipDetails() {
    Vector2 center = position;
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

    const char* name = "Pirate";
    DrawText(name,
        static_cast<int>(center.x - MeasureText(name, 20) / 2),
        static_cast<int>(center.y - 30),
        20,
        WHITE);

    if (reloadTime > 0.0f) {
        char reloadText[32];
        sprintf_s(reloadText, sizeof(reloadText), "Reload: %.1f", reloadTime);
        DrawText(reloadText,
            static_cast<int>(center.x - MeasureText(reloadText, 20) / 2),
            static_cast<int>(center.y - 50),
            20,
            YELLOW);
    }

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
