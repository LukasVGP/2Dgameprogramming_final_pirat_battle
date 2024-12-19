#include "pirateship.h"
#include <cmath>
#include <algorithm>

PirateShip::PirateShip() {
    position = { 200, 600 };
    targetPosition = position;
    rotation = 0.0f;
    targetRotation = 0.0f;
    currentSpeed = 0.0f;
    health = 100.0f;
    maxHealth = 100.0f;
    isSinking = false;
    sinkProgress = 0.0f;
    reloadTimer = 0.0f;
}

void PirateShip::Update() {
    if (isSinking) {
        sinkProgress += 0.01f;
        return;
    }

    UpdateAI();
    UpdatePhysics();
    UpdateCannonballs();

    if (reloadTimer > 0) {
        reloadTimer -= GetFrameTime();
    }
}

void PirateShip::UpdateAI() {
    float distanceToTarget = GetDistanceTo(targetShipPos);
    isInCombat = distanceToTarget < ATTACK_RANGE;

    if (isInCombat) {
        float angleToTarget = GetAngleTo(targetShipPos);
        targetRotation = angleToTarget;

        if (distanceToTarget < PREFERRED_RANGE) {
            currentSpeed = -MAX_SPEED * 0.3f;
        }
        else if (distanceToTarget > PREFERRED_RANGE) {
            currentSpeed = MAX_SPEED * 0.6f;
        }

        if (reloadTimer <= 0 && fabs(angleToTarget - rotation) < PI / 4) {
            AttemptToShoot();
        }
    }
    else {
        float patrolRadius = 150.0f;
        float time = GetTime() * 0.25f;

        Vector2 nextPosition;
        nextPosition.x = 200 + cosf(time) * patrolRadius;
        nextPosition.y = 600 + sinf(time) * patrolRadius;

        // Calculate tangent direction
        Vector2 tangent;
        tangent.x = -sinf(time);
        tangent.y = cosf(time);

        // Set target position slightly ahead on the patrol path
        targetPosition.x = nextPosition.x + tangent.x * 30.0f;
        targetPosition.y = nextPosition.y + tangent.y * 30.0f;

        float angleToTarget = atan2f(tangent.y, tangent.x);
        float angleDiff = angleToTarget - rotation;

        while (angleDiff > PI) angleDiff -= 2 * PI;
        while (angleDiff < -PI) angleDiff += 2 * PI;

        targetRotation = rotation + angleDiff * 0.1f;
        float turnFactor = 1.0f - fabs(angleDiff) / PI;
        currentSpeed = MAX_SPEED * (0.5f + 0.5f * turnFactor);

        position = nextPosition;
    }
}

void PirateShip::UpdatePhysics() {
    float deltaTime = GetFrameTime();

    float angleDiff = targetRotation - rotation;
    while (angleDiff > PI) angleDiff -= 2 * PI;
    while (angleDiff < -PI) angleDiff += 2 * PI;

    rotation += angleDiff * TURN_RATE * deltaTime;

    Vector2 forward = {
        cosf(rotation),
        sinf(rotation)
    };

    velocity.x = forward.x * currentSpeed;
    velocity.y = forward.y * currentSpeed;

    if (!isInCombat) {
        float lerpFactor = 0.1f;
        position.x = lerp(position.x, targetPosition.x, lerpFactor);
        position.y = lerp(position.y, targetPosition.y, lerpFactor);
    }
    else {
        position.x += velocity.x;
        position.y += velocity.y;
    }

    position.x = Clamp(position.x, 0, 1200);
    position.y = Clamp(position.y, 0, 1200);
}

void PirateShip::Draw() {
    Color mainColor = isSinking ? ColorAlpha(MAROON, 1.0f - sinkProgress) : MAROON;
    Vector2 center = position;
    float rot = rotation * RAD2DEG;
    Vector2 rotatedOffset = { cosf(rotation), sinf(rotation) };

    DrawRectanglePro(
        Rectangle{ center.x, center.y, 70, 25 },
        Vector2{ 35, 12.5f },
        rot,
        mainColor
    );

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

    DrawRectanglePro(
        Rectangle{ center.x - rotatedOffset.x * 30, center.y - rotatedOffset.y * 30, 30, 35 },
        Vector2{ 15, 17.5f },
        rot,
        DARKBROWN
    );

    for (int i = -1; i <= 1; i += 2) {
        float mastOffset = i * 20;
        DrawLineEx(
            Vector2{ center.x + rotatedOffset.x * mastOffset, center.y + rotatedOffset.y * mastOffset },
            Vector2{ center.x + rotatedOffset.x * mastOffset, center.y + rotatedOffset.y * mastOffset - 45 },
            4.0f,
            DARKBROWN
        );

        DrawTriangle(
            Vector2{ center.x + rotatedOffset.x * mastOffset, center.y + rotatedOffset.y * mastOffset - 45 },
            Vector2{ center.x + rotatedOffset.x * (mastOffset - 25), center.y + rotatedOffset.y * mastOffset - 10 },
            Vector2{ center.x + rotatedOffset.x * (mastOffset + 25), center.y + rotatedOffset.y * mastOffset - 10 },
            BLACK
        );
    }

    DrawRectanglePro(
        Rectangle{ center.x, center.y - 50, 25, 20 },
        Vector2{ 12.5f, 0 },
        rot,
        BLACK
    );

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

    Vector2 arrowStart = position;
    float arrowLength = 45.0f;
    Vector2 leftArrow = {
        arrowStart.x + cosf(rotation - PI / 2) * arrowLength,
        arrowStart.y + sinf(rotation - PI / 2) * arrowLength
    };
    Vector2 rightArrow = {
        arrowStart.x + cosf(rotation + PI / 2) * arrowLength,
        arrowStart.y + sinf(rotation + PI / 2) * arrowLength
    };

    DrawLineEx(arrowStart, leftArrow, 3.0f, RED);
    DrawLineEx(arrowStart, rightArrow, 3.0f, RED);

    for (const auto& ball : cannonballs) {
        if (ball.active) {
            DrawCircleV(ball.pos, 3.0f, BLACK);
        }
    }
}

bool PirateShip::IsPointInCannonRange(Vector2 point) {
    float expandedRadius = 30.0f;
    Vector2 leftCannon = {
        position.x + cosf(rotation - PI / 2) * 15.0f,
        position.y + sinf(rotation - PI / 2) * 15.0f
    };
    Vector2 rightCannon = {
        position.x + cosf(rotation + PI / 2) * 15.0f,
        position.y + sinf(rotation + PI / 2) * 15.0f
    };

    return (CheckCollisionPointCircle(point, leftCannon, expandedRadius) ||
        CheckCollisionPointCircle(point, rightCannon, expandedRadius));
}

void PirateShip::AttemptToShoot() {
    if (GetRandomValue(0, 100) < ACCURACY * 100) {
        float leftAngle = rotation - PI / 2;
        float rightAngle = rotation + PI / 2;

        for (int i = 0; i < 3; i++) {
            Cannonball leftBall;
            leftBall.pos = position;
            leftBall.velocity = {
                cosf(leftAngle) * 5.0f,
                sinf(leftAngle) * 5.0f
            };
            leftBall.active = true;
            cannonballs.push_back(leftBall);

            Cannonball rightBall;
            rightBall.pos = position;
            rightBall.velocity = {
                cosf(rightAngle) * 5.0f,
                sinf(rightAngle) * 5.0f
            };
            rightBall.active = true;
            cannonballs.push_back(rightBall);
        }
    }

    reloadTimer = RELOAD_TIME;
}

void PirateShip::UpdateCannonballs() {
    for (auto& ball : cannonballs) {
        if (ball.active) {
            ball.pos.x += ball.velocity.x;
            ball.pos.y += ball.velocity.y;

            float distanceTraveled = sqrtf(
                (ball.pos.x - position.x) * (ball.pos.x - position.x) +
                (ball.pos.y - position.y) * (ball.pos.y - position.y)
            );

            if (ball.pos.x < 0 || ball.pos.x > 1200 ||
                ball.pos.y < 0 || ball.pos.y > 1200 ||
                distanceTraveled > MAX_SHOOT_RANGE) {
                ball.active = false;
            }
        }
    }

    cannonballs.erase(
        std::remove_if(cannonballs.begin(), cannonballs.end(),
            [](const Cannonball& ball) { return !ball.active; }),
        cannonballs.end()
    );
}

void PirateShip::TakeDamage(float amount) {
    health -= amount;
    if (health <= 0 && !isSinking) {
        health = 0;
        isSinking = true;
        sinkProgress = 0.0f;
    }
}

float PirateShip::GetAngleTo(Vector2 target) const {
    return atan2f(target.y - position.y, target.x - position.x);
}

float PirateShip::GetDistanceTo(Vector2 target) const {
    float dx = target.x - position.x;
    float dy = target.y - position.y;
    return sqrtf(dx * dx + dy * dy);
}

float PirateShip::lerp(float start, float end, float amount) {
    return start + (end - start) * amount;
}

float PirateShip::Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
