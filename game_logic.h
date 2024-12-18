#pragma once
#include "raylib.h"
#include "navyship.h"
#include "pirateship.h"
#include "ui.h"
#include "environment.h"
#include "menu.h"
#include "game_states.h"
#include "cannon_ui.h"

class GameLogic {
public:
    GameLogic();
    void Update();
    void Draw();
    void Reset();

private:
    NavyShip playerShip;
    PirateShip enemyShip;
    UI ui;
    Environment env;
    Menu menu;
    CannonLoadingUI cannonUI;
    GameState currentState;
};
