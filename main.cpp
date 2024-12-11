#include "raylib.h"
#include "game_logic.h"

int main() {
    const int screenWidth = 1500;
    const int screenHeight = 1200;

    InitWindow(screenWidth, screenHeight, "Naval Game");
    GameLogic game;

    while (!WindowShouldClose()) {
        game.Update();
        BeginDrawing();
        ClearBackground(BLUE);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
