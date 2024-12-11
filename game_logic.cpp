#include "game_logic.h"
#include "raylib.h"
#include <cmath>

GameLogic::GameLogic() :
    playerShip(Vector2{ 600.0f, 300.0f }),
    enemyShip(),
    ui(),
    env() {
}

void GameLogic::Update() {
    ui.Update();
    float steering = ui.GetSteering();
    float throttle = ui.GetThrottle();
    bool firePressed = ui.IsFirePressed();

    playerShip.SetSteering(steering);
    playerShip.SetThrottle(throttle);
    if (firePressed) {
        playerShip.Shoot();
    }

    playerShip.Update();
    enemyShip.Update();

    const auto& playerBalls = playerShip.GetCannonballs();
    for (const auto& ball : playerBalls) {
        if (ball.active) {
            if (CheckCollisionCircleRec(
                ball.pos, 3,
                Rectangle{
                    enemyShip.GetPosition().x - 20,
                    enemyShip.GetPosition().y - 10,
                    40, 20
                })) {
                enemyShip.TakeDamage(35.0f / 3.0f);
            }
        }
    }

    const auto& enemyBalls = enemyShip.GetCannonballs();
    for (const auto& ball : enemyBalls) {
        if (ball.active) {
            if (CheckCollisionCircleRec(
                ball.pos, 3,
                Rectangle{
                    playerShip.GetPosition().x - 20,
                    playerShip.GetPosition().y - 10,
                    40, 20
                })) {
                playerShip.TakeDamage(35.0f / 3.0f);
            }
        }
    }
}

void GameLogic::Draw() {
    ClearBackground(BLUE);
    DrawRectangleLines(0, 0, 1200, 1200, WHITE);

    env.Draw();

    if (!playerShip.IsSinking()) {
        playerShip.Draw();
    }

    if (!enemyShip.IsSinking()) {
        enemyShip.Draw();
    }

    ui.Draw();

    DrawFPS(10, 10);
    DrawText("Use WHEEL to steer", 10, 30, 20, WHITE);
    DrawText("Use LEVER to control speed", 10, 50, 20, WHITE);
    DrawText("FIRE button for broadsides", 10, 70, 20, WHITE);

    if (playerShip.IsSinking()) {
        DrawText("DEFEAT - Pirates Win!", 500, 600, 40, RED);
    }

    if (enemyShip.IsSinking()) {
        DrawText("VICTORY - Navy Wins!", 500, 600, 40, GREEN);
    }
}
