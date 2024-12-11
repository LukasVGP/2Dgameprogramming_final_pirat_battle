#include "ui.h"

UI::UI() :
    wheelBounds({ 1250, 100, 200, 200 }),
    leverBounds({ 1300, 350, 50, 200 }),
    fireBounds({ 1275, 600, 150, 100 }),
    steering(0.0f),
    throttle(0.0f),
    firePressed(false) {}

void UI::Update() {
    if (CheckCollisionPointRec(GetMousePosition(), wheelBounds) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        steering = (GetMousePosition().x - wheelBounds.x - wheelBounds.width / 2) / (wheelBounds.width / 2);
    }

    if (CheckCollisionPointRec(GetMousePosition(), leverBounds) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        throttle = (leverBounds.y + leverBounds.height - GetMousePosition().y) / leverBounds.height;
        throttle = Clamp(throttle, 0.0f, 1.0f);
    }

    firePressed = CheckCollisionPointRec(GetMousePosition(), fireBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void UI::Draw() {
    DrawRectangleRounded({ 1210, 10, 280, 1180 }, 0.1f, 10, ColorAlpha(GRAY, 0.8f));
    DrawRectangleRoundedLines({ 1210, 10, 280, 1180 }, 0.1f, 10, 2, DARKGRAY);

    DrawText("SHIP CONTROLS", 1265, 30, 24, DARKGRAY);
    DrawLine(1230, 60, 1470, 60, DARKGRAY);

    Vector2 wheelCenter = { wheelBounds.x + wheelBounds.width / 2,
                         wheelBounds.y + wheelBounds.height / 2 };

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
            { wheelCenter.x + cosf(angle) * 90,
             wheelCenter.y + sinf(angle) * 90 },
            5, BLACK
        );
    }

    DrawCircle(wheelCenter.x, wheelCenter.y, 15, DARKGRAY);
    DrawCircle(wheelCenter.x, wheelCenter.y, 10, BLACK);
    DrawText("STEERING", wheelCenter.x - 40, wheelBounds.y + wheelBounds.height + 10, 20, DARKGRAY);

    DrawRectangleRounded(leverBounds, 0.5f, 8, DARKGRAY);
    DrawRectangleRoundedLines(leverBounds, 0.5f, 8, 2, BLACK);

    float leverY = leverBounds.y + leverBounds.height * (1 - throttle);
    DrawRectangleRounded(
        { leverBounds.x - 10, leverY - 10, 70, 20 },
        0.5f, 8, RED
    );
    DrawRectangleRoundedLines(
        { leverBounds.x - 10, leverY - 10, 70, 20 },
        0.5f, 8, 2, MAROON
    );

    DrawText("THROTTLE", leverBounds.x - 20, leverBounds.y + leverBounds.height + 10, 20, DARKGRAY);

    Color fireColor = firePressed ? RED : MAROON;
    DrawRectangleRounded(fireBounds, 0.2f, 8, fireColor);
    DrawRectangleRoundedLines(fireBounds, 0.2f, 8, 2, BLACK);

    DrawText("FIRE!", fireBounds.x + 37, fireBounds.y + 37, 30, BLACK);
    DrawText("FIRE!", fireBounds.x + 35, fireBounds.y + 35, 30, WHITE);

    DrawText("BROADSIDE READY", fireBounds.x + 10, fireBounds.y + 70, 15,
        firePressed ? DARKGRAY : GREEN);
}
