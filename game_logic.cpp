#include "game_logic.h"
#include "raylib.h"
#include <cmath>

GameLogic::GameLogic() :
    playerShip(Vector2{ 1000.0f, 600.0f }),
    enemyShip(),
    ui(),
    env(),
    menu(),
    currentState(GameState::MENU) {
}

void GameLogic::Update() {
    switch (currentState) {
    case GameState::MENU:
        if (menu.IsStartPressed()) {
            currentState = GameState::PLAYING;
        }
        break;

    case GameState::PLAYING: {
        ui.Update();

        float steering = ui.GetSteering();
        float throttle = ui.GetThrottle();
        bool firePressed = ui.IsFirePressed();

        playerShip.SetSteering(steering);
        playerShip.SetThrottle(throttle);
        playerShip.SetCannonReadyState(
            ui.IsLeftCannonReady(),
            ui.IsRightCannonReady()
        );

        if (firePressed) {
            playerShip.Shoot();
            ui.ResetCannons();
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

        if (playerShip.IsSinking() || enemyShip.IsSinking()) {
            currentState = GameState::GAMEOVER;
        }
        break;
    }

    case GameState::GAMEOVER:
        if (menu.IsRestartPressed()) {
            Reset();
            currentState = GameState::PLAYING;
        }
        break;
    }
}

void GameLogic::Draw() {
    ClearBackground(BLUE);

    switch (currentState) {
    case GameState::MENU:
        menu.Draw();
        break;

    case GameState::PLAYING:
        env.Draw();
        if (!playerShip.IsSinking()) playerShip.Draw();
        if (!enemyShip.IsSinking()) enemyShip.Draw();
        ui.Draw();

        DrawFPS(10, 10);
        DrawText("Use WHEEL to steer", 10, 30, 20, WHITE);
        DrawText("Use LEVER to control speed", 10, 50, 20, WHITE);
        DrawText("Load cannons and FIRE!", 10, 70, 20, WHITE);
        break;

    case GameState::GAMEOVER:
        env.Draw();
        if (!playerShip.IsSinking()) playerShip.Draw();
        if (!enemyShip.IsSinking()) enemyShip.Draw();
        menu.DrawGameOver(!playerShip.IsSinking());
        break;
    }
}

void GameLogic::Reset() {
    playerShip = NavyShip(Vector2{ 1000.0f, 600.0f });
    enemyShip = PirateShip();
    ui.ResetCannons();
}
