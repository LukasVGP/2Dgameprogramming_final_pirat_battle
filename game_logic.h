#pragma once
#include "raylib.h"
#include "navyship.h"
#include "pirateship.h"
#include "ui.h"
#include "environment.h"

class GameLogic {
public:
    GameLogic();
    void Update();
    void Draw();

private:
    NavyShip playerShip;
    PirateShip enemyShip;
    UI ui;
    Environment env;
};
