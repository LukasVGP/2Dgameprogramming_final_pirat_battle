#include "ui.h"

UI::UI() {
    wheelBounds = { 1250, 100, 200, 200 };
    leverBounds = { 1300, 350, 50, 200 };
    fireBounds = { 1275, 600, 150, 100 };
    steering = 0.0f;
    throttle = 0.0f;
    firePressed = false;

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
        startY + CANNON_SPACING + 80.0f,
        CANNON_WIDTH,
        CANNON_HEIGHT
    };

    float stackX = startX + 20.0f;
    float stackY = startY + 40.0f;

    leftCannon.ballStackBounds = rightCannon.ballStackBounds = {
        stackX,
        stackY,
        60.0f,
        80.0f
    };

    leftCannon.ramrodBounds = rightCannon.ramrodBounds = {
        startX - 60.0f,
        startY + ((CANNON_SPACING + 80.0f) / 2),
        80.0f,
        20.0f
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
    ImageDrawCircle(&ballImg, 12, 12, 9, BLACK);
    cannonballTexture = LoadTextureFromImage(ballImg);
    UnloadImage(ballImg);

    Image ramrodImg = GenImageColor(60, 16, BROWN);
    for (int i = 0; i < 60; i += 8) {
        ImageDrawRectangle(&ramrodImg, i, 0, 4, 16, DARKBROWN);
    }
    ImageDrawRectangle(&ramrodImg, 45, 2, 15, 12, GRAY);
    ramrodTexture = LoadTextureFromImage(ramrodImg);
    UnloadImage(ramrodImg);
}

void UI::Update() {
    if (CheckCollisionPointRec(GetMousePosition(), wheelBounds) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        steering = (GetMousePosition().x - wheelBounds.x - wheelBounds.width / 2) / (wheelBounds.width / 2);
    }

    if (CheckCollisionPointRec(GetMousePosition(), leverBounds) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        throttle = (leverBounds.y + leverBounds.height - GetMousePosition().y) / leverBounds.height;
        throttle = Clamp(throttle, 0.0f, 1.0f);
    }

    firePressed = CheckCollisionPointRec(GetMousePosition(), fireBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    UpdateCannonDragging(leftCannon);
    UpdateCannonDragging(rightCannon);
}

void UI::Draw() {
    DrawRectangleRounded({ 1210, 10, 280, 1180 }, 0.1f, 10, ColorAlpha(WHITE, 0.8f));
    DrawRectangleRounded({ 1210, 10, 280, 1180 }, 0.1f, 10, WHITE);
    DrawShipControls();
    DrawCannonControls();
}

void UI::DrawShipControls() {
    DrawText("SHIP CONTROLS", 1265, 30, 24, DARKGRAY);
    DrawLine(1230, 60, 1470, 60, DARKGRAY);

    Vector2 wheelCenter = {
        wheelBounds.x + wheelBounds.width / 2,
        wheelBounds.y + wheelBounds.height / 2
    };

    DrawCircle(wheelCenter.x, wheelCenter.y, wheelBounds.width / 2, DARKGRAY);
    DrawCircle(wheelCenter.x, wheelCenter.y, wheelBounds.width / 2 - 10, LIGHTGRAY);

    float arrowRotation = steering * PI;
    Vector2 arrowTop = {
        wheelCenter.x + sinf(arrowRotation) * (wheelBounds.width / 2 - 5),
        wheelCenter.y - cosf(arrowRotation) * (wheelBounds.width / 2 - 5)
    };
    Vector2 arrowLeft = {
        wheelCenter.x + sinf(arrowRotation - 0.3f) * (wheelBounds.width / 2 - 25),
        wheelCenter.y - cosf(arrowRotation - 0.3f) * (wheelBounds.width / 2 - 25)
    };
    Vector2 arrowRight = {
        wheelCenter.x + sinf(arrowRotation + 0.3f) * (wheelBounds.width / 2 - 25),
        wheelCenter.y - cosf(arrowRotation + 0.3f) * (wheelBounds.width / 2 - 25)
    };

    DrawTriangle(arrowTop, arrowLeft, arrowRight, RED);
    DrawTriangleLines(arrowTop, arrowLeft, arrowRight, MAROON);

    for (int i = 0; i < 8; i++) {
        float angle = i * PI / 4 + steering * PI / 4;
        DrawLineEx(
            { wheelCenter.x, wheelCenter.y },
            { wheelCenter.x + cosf(angle) * 90, wheelCenter.y + sinf(angle) * 90 },
            5, BLACK
        );
    }

    DrawCircle(wheelCenter.x, wheelCenter.y, 15, DARKGRAY);
    DrawCircle(wheelCenter.x, wheelCenter.y, 10, BLACK);
    DrawText("STEERING", wheelCenter.x - 40, wheelBounds.y + wheelBounds.height + 10, 20, DARKGRAY);

    DrawRectangleRounded(leverBounds, 0.5f, 8, DARKGRAY);
    DrawRectangleRounded(leverBounds, 0.5f, 8, BLACK);

    float leverY = leverBounds.y + leverBounds.height * (1 - throttle);
    Rectangle leverHandle = { leverBounds.x - 10, leverY - 10, 70, 20 };
    DrawRectangleRounded(leverHandle, 0.5f, 8, RED);
    DrawRectangleRounded(leverHandle, 0.5f, 8, MAROON);

    DrawText("THROTTLE", leverBounds.x - 20, leverBounds.y + leverBounds.height + 10, 20, DARKGRAY);

    Color fireColor = firePressed ? RED : MAROON;
    DrawRectangleRounded(fireBounds, 0.2f, 8, fireColor);
    DrawRectangleRounded(fireBounds, 0.2f, 8, BLACK);
    DrawText("FIRE!", fireBounds.x + 37, fireBounds.y + 37, 30, BLACK);
    DrawText("FIRE!", fireBounds.x + 35, fireBounds.y + 35, 30, WHITE);
    DrawText("BROADSIDE READY", fireBounds.x + 10, fireBounds.y + 70, 15, firePressed ? DARKGRAY : GREEN);
}

void UI::DrawCannonControls() {
    DrawText("CANNON LOADING", 1265, 710, 24, DARKGRAY);
    DrawCannon(leftCannon);
    DrawCannon(rightCannon);
}

void UI::DrawCannon(const CannonUI& cannon) {
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

    const char* sideText = cannon.isLeftCannon ? "PORT" : "STARBOARD";
    DrawText(sideText,
        cannon.bounds.x - 30,
        cannon.bounds.y - 40,
        15,
        RED);

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
        DrawText("ROUND SHOT",
            cannon.ballStackBounds.x - 20,
            cannon.ballStackBounds.y - 20,
            15,
            DARKGRAY);

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

        DrawText("RAMROD",
            cannon.ramrodBounds.x,
            cannon.ramrodBounds.y - 20,
            15,
            DARKGRAY);

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

void UI::UpdateCannonDragging(CannonUI& cannon) {
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

void UI::ResetCannons() {
    leftCannon.state = CannonState::EMPTY;
    rightCannon.state = CannonState::EMPTY;
}

float UI::Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
