#include "cannon_ui.h"

CannonLoadingUI::CannonLoadingUI() {
    float startY = 800.0f;
    float startX = 1350.0f;

    leftCannon.bounds = {
        startX,
        startY,
        CANNON_WIDTH,
        CANNON_HEIGHT
    };

    rightCannon.bounds = {
        startX,
        startY + CANNON_SPACING,
        CANNON_WIDTH,
        CANNON_HEIGHT
    };

    float stackX = startX + 20.0f;
    float stackY = startY + (1.5f * CANNON_SPACING);

    leftCannon.ramrodBounds = rightCannon.ramrodBounds = {
        stackX - 40.0f,
        startY + CANNON_SPACING - 20.0f,
        80.0f,
        20.0f
    };

    leftCannon.ballStackBounds = rightCannon.ballStackBounds = {
        stackX,
        stackY,
        60.0f,
        80.0f
    };

    leftCannon.state = CannonState::EMPTY;
    leftCannon.isLeftCannon = true;
    leftCannon.rotation = 0;
    leftCannon.isDraggingBall = false;
    leftCannon.isDraggingRamrod = false;

    rightCannon.state = CannonState::EMPTY;
    rightCannon.isLeftCannon = false;
    rightCannon.rotation = 0;
    rightCannon.isDraggingBall = false;
    rightCannon.isDraggingRamrod = false;

    Image ballImg = GenImageColor(24, 24, BLANK);
    ImageDrawCircle(&ballImg, 12, 12, 11, BLACK);
    ImageDrawCircle(&ballImg, 12, 12, 9, DARKGRAY);
    cannonballTexture = LoadTextureFromImage(ballImg);
    UnloadImage(ballImg);

    Image ramrodImg = GenImageChecked(60, 16, 1, 1, BROWN, DARKBROWN);
    ramrodTexture = LoadTextureFromImage(ramrodImg);
    UnloadImage(ramrodImg);
}

void CannonLoadingUI::Update() {
    UpdateCannonDragging(leftCannon);
    UpdateCannonDragging(rightCannon);
}

void CannonLoadingUI::UpdateCannonDragging(CannonUI& cannon) {
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, cannon.ballStackBounds) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        cannon.state == CannonState::EMPTY) {
        cannon.isDraggingBall = true;
        cannon.dragOffset = {
            mousePos.x - cannon.ballStackBounds.x,
            mousePos.y - cannon.ballStackBounds.y
        };
    }

    if (cannon.isDraggingBall && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos, cannon.bounds)) {
            cannon.state = CannonState::LOADED_BALL;
        }
        cannon.isDraggingBall = false;
    }

    if (CheckCollisionPointRec(mousePos, cannon.ramrodBounds) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        cannon.state == CannonState::LOADED_BALL) {
        cannon.isDraggingRamrod = true;
        cannon.dragOffset = {
            mousePos.x - cannon.ramrodBounds.x,
            mousePos.y - cannon.ramrodBounds.y
        };
    }

    if (cannon.isDraggingRamrod && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos, cannon.bounds)) {
            cannon.state = CannonState::READY_TO_FIRE;
        }
        cannon.isDraggingRamrod = false;
    }
}

void CannonLoadingUI::Draw() {
    DrawRectangle(1200, 600, 400, 400, Color{ 128, 0, 32, 255 });
    DrawText("CANNON LOADING", 1265, 710, 24, WHITE);
    DrawCannon(leftCannon);
    DrawCannon(rightCannon);
}

void CannonLoadingUI::DrawCannon(const CannonUI& cannon) {
    Color barrelColor = DARKGRAY;

    DrawRectanglePro(
        { cannon.bounds.x, cannon.bounds.y, cannon.bounds.width, cannon.bounds.height },
        { cannon.bounds.width / 2, cannon.bounds.height / 2 },
        cannon.rotation,
        barrelColor
    );

    DrawCircle(
        cannon.bounds.x - cannon.bounds.width / 2,
        cannon.bounds.y,
        cannon.bounds.height / 2,
        barrelColor
    );

    DrawCircle(
        cannon.bounds.x + cannon.bounds.width / 2,
        cannon.bounds.y,
        cannon.bounds.height / 2 + 5,
        barrelColor
    );

    for (int i = 1; i < 4; i++) {
        float x = cannon.bounds.x - cannon.bounds.width / 2 + (cannon.bounds.width * i / 4);
        DrawRectangle(x - 2, cannon.bounds.y - cannon.bounds.height / 2, 4, cannon.bounds.height, BLACK);
    }

    Vector2 arrowStart = {
        cannon.bounds.x + cannon.bounds.width + 20,
        cannon.bounds.y
    };
    Vector2 arrowEnd = {
        arrowStart.x + 40,
        arrowStart.y
    };
    DrawLineEx(arrowStart, arrowEnd, 3, YELLOW);
    DrawTriangle(
        { arrowEnd.x, arrowEnd.y },
        { arrowEnd.x - 10, arrowEnd.y - 10 },
        { arrowEnd.x - 10, arrowEnd.y + 10 },
        YELLOW
    );

    const char* stateText = "UNKNOWN";
    Color stateColor = WHITE;
    switch (cannon.state) {
    case CannonState::EMPTY:
        stateText = "EMPTY";
        stateColor = RED;
        break;
    case CannonState::LOADED_BALL:
        stateText = "LOAD RAMROD";
        stateColor = YELLOW;
        break;
    case CannonState::READY_TO_FIRE:
        stateText = "READY!";
        stateColor = GREEN;
        break;
    }

    DrawText(stateText,
        cannon.bounds.x,
        cannon.bounds.y + cannon.bounds.height / 2 + INDICATOR_OFFSET,
        15,
        stateColor);

    if (cannon.isLeftCannon) {
        int rows = 4;
        float ballSize = 20;
        float startX = cannon.ballStackBounds.x;
        float startY = cannon.ballStackBounds.y + cannon.ballStackBounds.height;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < (rows - row); col++) {
                float x = startX + (col * ballSize) + (row * ballSize / 2);
                float y = startY - (row * ballSize);
                DrawTexture(cannonballTexture, x, y, WHITE);
            }
        }
    }

    if (cannon.isLeftCannon) {
        DrawRectangleRec(cannon.ramrodBounds, BROWN);
        DrawCircle(
            cannon.ramrodBounds.x + cannon.ramrodBounds.width - 10,
            cannon.ramrodBounds.y + cannon.ramrodBounds.height / 2,
            12,
            DARKBROWN
        );
    }

    if (cannon.isDraggingBall) {
        Vector2 mousePos = GetMousePosition();
        DrawTexture(cannonballTexture,
            mousePos.x - cannon.dragOffset.x,
            mousePos.y - cannon.dragOffset.y,
            WHITE);
    }

    if (cannon.isDraggingRamrod) {
        Vector2 mousePos = GetMousePosition();
        DrawTexture(ramrodTexture,
            mousePos.x - cannon.dragOffset.x,
            mousePos.y - cannon.dragOffset.y,
            WHITE);
    }
}

void CannonLoadingUI::ResetCannons() {
    leftCannon.state = CannonState::EMPTY;
    rightCannon.state = CannonState::EMPTY;
}
