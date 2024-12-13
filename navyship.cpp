#include "navyship.h"
#include <algorithm>
#include "raymath.h"

NavyShip::NavyShip(Vector2 startPos) :
    position({ 800.0f, 600.0f }),
    rotation(PI),
    targetRotation(0.0f),
    targetPosition(startPos),
    velocity({ 0.0f, 0.0f }),
    angularVelocity(0.0f),
    targetSpeed(0.0f),
    health(100.0f),
    maxHealth(100.0f),
    reloadTime(0),
    isSinking(false),
    sinkProgress(0.0f) {
}

void NavyShip::Update() {
    if (isSinking) {
        sinkProgress += 0.01f;
        return;
    }
    UpdatePhysics();
    UpdateCannonballs();
    if (reloadTime > 0) {
        reloadTime--;
    }
}

void NavyShip::UpdatePhysics() {
    float deltaTime = GetFrameTime();
    float targetVelX = cosf(rotation) * targetSpeed;
    float targetVelY = sinf(rotation) * targetSpeed;
    velocity.x = lerp(velocity.x, targetVelX, ACCELERATION);
    velocity.y = lerp(velocity.y, targetVelY, ACCELERATION);
    velocity.x *= LINEAR_DAMPING;
    velocity.y *= LINEAR_DAMPING;
    position.x += velocity.x;
    position.y += velocity.y;
    position.x = Clamp(position.x, 0.0f, 1200.0f);
    position.y = Clamp(position.y, 0.0f, 1200.0f);
}

void NavyShip::Draw() {
    Color shipColor = isSinking ? ColorAlpha(DARKBLUE, 1.0f - sinkProgress) : DARKBLUE;
    Color sailColor = WHITE;
    Vector2 center = position;
    float rot = rotation * RAD2DEG;
    Vector2 rotatedOffset = { cosf(rotation), sinf(rotation) };

    // Main hull
    DrawRectanglePro(
        Rectangle{ center.x, center.y, 60, 20 },
        Vector2{ 30, 10 },
        rot,
        shipColor
    );

    // Bow (front)
    Vector2 bowTip = {
        center.x + rotatedOffset.x * 35,
        center.y + rotatedOffset.y * 35
    };
    DrawTriangle(
        Vector2{ center.x + rotatedOffset.x * 30, center.y + rotatedOffset.y * 30 - 10 },
        Vector2{ center.x + rotatedOffset.x * 30, center.y + rotatedOffset.y * 30 + 10 },
        bowTip,
        shipColor
    );

    // Stern (back)
    DrawRectanglePro(
        Rectangle{ center.x - rotatedOffset.x * 25, center.y - rotatedOffset.y * 25, 25, 30 },
        Vector2{ 12.5f, 15 },
        rot,
        DARKBLUE
    );

    // Three masts with triangle sails
    float mastSpacing = 20.0f;
    for (int i = 1; i >= -1; i--) {
        float mastOffset = i * mastSpacing;

        // Triangle sail
        DrawTriangle(
            Vector2{ center.x + rotatedOffset.x * mastOffset, center.y + rotatedOffset.y * mastOffset - 40 },
            Vector2{ center.x + rotatedOffset.x * (mastOffset + 15), center.y + rotatedOffset.y * mastOffset },
            Vector2{ center.x + rotatedOffset.x * (mastOffset - 15), center.y + rotatedOffset.y * mastOffset },
            WHITE
        );

        // Brown mast base
        DrawRectanglePro(
            Rectangle{ center.x + rotatedOffset.x * mastOffset, center.y + rotatedOffset.y * mastOffset, 6, 15 },
            Vector2{ 3, 0 },
            rot,
            BROWN
        );

        // Mast
        DrawLineEx(
            Vector2{ center.x + rotatedOffset.x * mastOffset, center.y + rotatedOffset.y * mastOffset },
            Vector2{ center.x + rotatedOffset.x * mastOffset, center.y + rotatedOffset.y * mastOffset - 40 },
            3.0f,
            BROWN
        );
    }

    // British Naval Ensign
    DrawRectanglePro(
        Rectangle{ center.x - rotatedOffset.x * 25, center.y - rotatedOffset.y * 25 - 35, 20, 15 },
        Vector2{ 0, 0 },
        rot,
        RED
    );

    DrawShipDetails();
}

void NavyShip::Shoot() {
    if (reloadTime <= 0) {
        Vector2 leftCannonPos = {
            position.x + cosf(rotation - PI / 2) * 10,
            position.y + sinf(rotation - PI / 2) * 10
        };
        Vector2 rightCannonPos = {
            position.x + cosf(rotation + PI / 2) * 10,
            position.y + sinf(rotation + PI / 2) * 10
        };
        Vector2 leftVelocity = {
            cosf(rotation - PI / 2) * 5.0f,
            sinf(rotation - PI / 2) * 5.0f
        };
        Vector2 rightVelocity = {
            cosf(rotation + PI / 2) * 5.0f,
            sinf(rotation + PI / 2) * 5.0f
        };

        Cannonball leftBall;
        leftBall.pos = leftCannonPos;
        leftBall.velocity = leftVelocity;
        leftBall.rotation = rotation - PI / 2;
        leftBall.distanceTraveled = 0.0f;
        leftBall.active = true;

        Cannonball rightBall;
        rightBall.pos = rightCannonPos;
        rightBall.velocity = rightVelocity;
        rightBall.rotation = rotation + PI / 2;
        rightBall.distanceTraveled = 0.0f;
        rightBall.active = true;

        cannonballs.push_back(leftBall);
        cannonballs.push_back(rightBall);
        reloadTime = RELOAD_DURATION;
    }
}

void NavyShip::SetSteering(float value) {
    if (value != 0) {
        float rotationSpeed = value * PI * TURN_RATE * GetFrameTime() * 2.0f;
        rotation += rotationSpeed;
        while (rotation > 2 * PI) rotation -= 2 * PI;
        while (rotation < 0) rotation += 2 * PI;
    }
}

void NavyShip::SetThrottle(float value) {
    float scaledSpeed = (value > 0) ?
        (value * 0.2f * MAX_SPEED) :
        (value * 0.1f * MAX_SPEED);
    targetSpeed = scaledSpeed;
}

void NavyShip::TakeDamage(float amount) {
    health -= amount;
    if (health <= 0) {
        isSinking = true;
    }
}

void NavyShip::UpdateCannonballs() {
    for (auto& ball : cannonballs) {
        if (ball.active) {
            ball.pos.x += ball.velocity.x;
            ball.pos.y += ball.velocity.y;
            ball.distanceTraveled += sqrtf(ball.velocity.x * ball.velocity.x +
                ball.velocity.y * ball.velocity.y);
            if (ball.pos.x < 0 || ball.pos.x > 1200 ||
                ball.pos.y < 0 || ball.pos.y > 1200 ||
                ball.distanceTraveled > 500.0f) {
                ball.active = false;
            }
        }
    }
}

void NavyShip::DrawShipDetails() {
    DrawRectangle(static_cast<int>(position.x - 20), static_cast<int>(position.y - 25), 40, 5, RED);
    DrawRectangle(static_cast<int>(position.x - 20), static_cast<int>(position.y - 25),
        static_cast<int>(40 * (health / maxHealth)), 5, GREEN);
    for (const auto& ball : cannonballs) {
        if (ball.active) {
            DrawCircle(static_cast<int>(ball.pos.x), static_cast<int>(ball.pos.y), 3, BLACK);
        }
    }
}

float NavyShip::Clamp(float value, float min, float max) {
    return std::min(std::max(value, min), max);
}

float NavyShip::lerp(float start, float end, float amount) {
    return start + amount * (end - start);
}

