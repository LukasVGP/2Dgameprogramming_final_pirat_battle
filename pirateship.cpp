#include "pirateship.h"
#include <cmath>

PirateShip::PirateShip() {
    position = { 200.0f, 600.0f };
    velocity = { 0.0f, 0.0f };
    rotation = 0.0f;
    health = 100.0f;
    maxHealth = 100.0f;
}

void PirateShip::Update() {
    if (isSinking) {
        sinkProgress += GetFrameTime();
        return;
    }

    UpdateAIMovement();
    UpdatePhysics();
    UpdateCannonballs();

    if (reloadTime > 0.0f) {
        reloadTime -= GetFrameTime();
    }

    if (health <= 0.0f && !isSinking) {
        isSinking = true;
        sinkProgress = 0.0f;
    }
}

void PirateShip::UpdatePhysics() {
    float currentSpeed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
    float targetVelocityX = cosf(rotation) * targetSpeed;
    float targetVelocityY = sinf(rotation) * targetSpeed;

    velocity.x = lerp(velocity.x, targetVelocityX, ACCELERATION);
    velocity.y = lerp(velocity.y, targetVelocityY, ACCELERATION);

    float angleDiff = targetRotation - rotation;
    while (angleDiff > PI) angleDiff -= 2 * PI;
    while (angleDiff < -PI) angleDiff += 2 * PI;

    angularVelocity += angleDiff * TURN_RATE;
    angularVelocity *= ANGULAR_DAMPING;
    rotation += angularVelocity;

    velocity.x *= LINEAR_DAMPING;
    velocity.y *= LINEAR_DAMPING;

    position.x += velocity.x;
    position.y += velocity.y;
}

void PirateShip::UpdateAIMovement() {
    circleTime += GetFrameTime() * CIRCLE_SPEED;

    float targetX = ISLAND_CENTER.x + cosf(circleTime) * CIRCLE_RADIUS;
    float targetY = ISLAND_CENTER.y + sinf(circleTime) * CIRCLE_RADIUS;

    float dx = targetX - position.x;
    float dy = targetY - position.y;
    targetRotation = atan2f(dy, dx);

    float distanceToTarget = sqrtf(dx * dx + dy * dy);
    targetSpeed = Clamp(distanceToTarget / 100.0f, 0.0f, MAX_SPEED);
}

void PirateShip::Draw() {
    if (isSinking) {
        float sinkOffset = sinkProgress * 50.0f;
        DrawShipDetails();
        return;
    }

    DrawShipDetails();

    for (const auto& cannonball : cannonballs) {
        DrawCircle(static_cast<int>(cannonball.pos.x),
            static_cast<int>(cannonball.pos.y),
            3.0f,
            BLACK);
    }
}

void PirateShip::DrawShipDetails() {
    float baseWidth = 40.0f;
    float baseHeight = 20.0f;

    Vector2 points[4] = {
        { position.x + cosf(rotation) * baseWidth - sinf(rotation) * baseHeight,
          position.y + sinf(rotation) * baseWidth + cosf(rotation) * baseHeight },
        { position.x + cosf(rotation) * baseWidth + sinf(rotation) * baseHeight,
          position.y + sinf(rotation) * baseWidth - cosf(rotation) * baseHeight },
        { position.x - cosf(rotation) * baseWidth + sinf(rotation) * baseHeight,
          position.y - sinf(rotation) * baseWidth - cosf(rotation) * baseHeight },
        { position.x - cosf(rotation) * baseWidth - sinf(rotation) * baseHeight,
          position.y - sinf(rotation) * baseWidth + cosf(rotation) * baseHeight }
    };

    DrawTriangle(points[0], points[1], points[2], BROWN);
    DrawTriangle(points[0], points[2], points[3], BROWN);

    DrawLineEx(points[0], points[1], 2.0f, BLACK);
    DrawLineEx(points[1], points[2], 2.0f, BLACK);
    DrawLineEx(points[2], points[3], 2.0f, BLACK);
    DrawLineEx(points[3], points[0], 2.0f, BLACK);

    float healthBarWidth = 40.0f;
    float healthBarHeight = 5.0f;
    float healthPercentage = health / maxHealth;

    DrawRectangle(
        static_cast<int>(position.x - healthBarWidth / 2),
        static_cast<int>(position.y - 30),
        static_cast<int>(healthBarWidth),
        static_cast<int>(healthBarHeight),
        RED
    );

    DrawRectangle(
        static_cast<int>(position.x - healthBarWidth / 2),
        static_cast<int>(position.y - 30),
        static_cast<int>(healthBarWidth * healthPercentage),
        static_cast<int>(healthBarHeight),
        GREEN
    );
}

void PirateShip::Shoot() {
    if (reloadTime <= 0.0f) {
        float cannonOffsetX = -sinf(rotation) * 20.0f;
        float cannonOffsetY = cosf(rotation) * 20.0f;

        Cannonball leftCannonball;
        leftCannonball.pos = {
            position.x + cannonOffsetX,
            position.y + cannonOffsetY
        };
        leftCannonball.velocity = {
            -sinf(rotation) * 5.0f,
            cosf(rotation) * 5.0f
        };
        leftCannonball.active = true;
        leftCannonball.rotation = rotation;
        leftCannonball.distanceTraveled = 0.0f;

        Cannonball rightCannonball;
        rightCannonball.pos = {
            position.x - cannonOffsetX,
            position.y - cannonOffsetY
        };
        rightCannonball.velocity = {
            sinf(rotation) * 5.0f,
            -cosf(rotation) * 5.0f
        };
        rightCannonball.active = true;
        rightCannonball.rotation = rotation;
        rightCannonball.distanceTraveled = 0.0f;

        cannonballs.push_back(leftCannonball);
        cannonballs.push_back(rightCannonball);

        reloadTime = RELOAD_DURATION;
    }
}

void PirateShip::UpdateCannonballs() {
    for (auto& cannonball : cannonballs) {
        if (cannonball.active) {
            cannonball.pos.x += cannonball.velocity.x;
            cannonball.pos.y += cannonball.velocity.y;
            cannonball.distanceTraveled += sqrtf(
                cannonball.velocity.x * cannonball.velocity.x +
                cannonball.velocity.y * cannonball.velocity.y
            );
        }
    }

    cannonballs.erase(
        std::remove_if(
            cannonballs.begin(),
            cannonballs.end(),
            [](const Cannonball& c) { return c.distanceTraveled >= 300.0f; }
        ),
        cannonballs.end()
    );
}

void PirateShip::TakeDamage(float amount) {
    health -= amount;
    if (health < 0.0f) health = 0.0f;
}

float PirateShip::lerp(float start, float end, float amount) {
    return start + (end - start) * amount;
}

float PirateShip::Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
