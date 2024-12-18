#include "cannon_ui.h"

CannonLoadingUI::CannonLoadingUI() {
    // Initialize left cannon
    leftCannon.bounds = { 1230, 750, 100, 50 };
    leftCannon.ballStackBounds = { 1230, 810, 30, 60 };
    leftCannon.ramrodBounds = { 1270, 810, 60, 20 };
    leftCannon.state = CannonState::EMPTY;
    leftCannon.isLeftCannon = true;
    leftCannon.rotation = 180;
    leftCannon.isDraggingBall = false;
    leftCannon.isDraggingRamrod = false;

    // Initialize right cannon
    rightCannon.bounds = { 1350, 750, 100, 50 };
    rightCannon.ballStackBounds = { 1350, 810, 30, 60 };
    rightCannon.ramrodBounds = { 1390, 810, 60, 20 };
    rightCannon.state = CannonState::EMPTY;
    rightCannon.isLeftCannon = false;
    rightCannon.rotation = 0;
    rightCannon.isDraggingBall = false;
    rightCannon.isDraggingRamrod = false;

    // Load textures
    Image ballImg = GenImageChecked(20, 20, 1, 1, BLACK, DARKGRAY);
    cannonballTexture = LoadTextureFromImage(ballImg);
    UnloadImage(ballImg);

    Image ramrodImg = GenImageChecked(40, 10, 1, 1, BROWN, DARKBROWN);
    ramrodTexture = LoadTextureFromImage(ramrodImg);
    UnloadImage(ramrodImg);
}

void CannonLoadingUI::Update() {
    UpdateCannonDragging(leftCannon);
    UpdateCannonDragging(rightCannon);
}

void CannonLoadingUI::UpdateCannonDragging(CannonUI& cannon) {
    Vector2 mousePos = GetMousePosition();

    // Handle cannonball dragging
    if (CheckCollisionPointRec(mousePos, cannon.ballStackBounds) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        cannon.state == CannonState::EMPTY) {
        cannon.isDraggingBall = true;
        cannon.dragOffset = { mousePos.x - cannon.ballStackBounds.x,
                           mousePos.y - cannon.ballStackBounds.y };
    }

    if (cannon.isDraggingBall && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos, cannon.bounds)) {
            cannon.state = CannonState::LOADED_BALL;
        }
        cannon.isDraggingBall = false;
    }

    // Handle ramrod dragging
    if (CheckCollisionPointRec(mousePos, cannon.ramrodBounds) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        cannon.state == CannonState::LOADED_BALL) {
        cannon.isDraggingRamrod = true;
        cannon.dragOffset = { mousePos.x - cannon.ramrodBounds.x,
                           mousePos.y - cannon.ramrodBounds.y };
    }

    if (cannon.isDraggingRamrod && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos, cannon.bounds)) {
            cannon.state = CannonState::READY_TO_FIRE;
        }
        cannon.isDraggingRamrod = false;
    }
}

void CannonLoadingUI::Draw() {
    DrawText("CANNON LOADING", 1265, 710, 24, DARKGRAY);

    DrawCannon(leftCannon);
    DrawCannon(rightCannon);
}

void CannonLoadingUI::DrawCannon(const CannonUI& cannon) {
    // Draw cannon
    DrawRectanglePro(
        { cannon.bounds.x, cannon.bounds.y, cannon.bounds.width, cannon.bounds.height },
        { cannon.bounds.width / 2, cannon.bounds.height / 2 },
        cannon.rotation,
        DARKGRAY
    );

    // Draw cannonball stack
    DrawRectangleRec(cannon.ballStackBounds, DARKGRAY);
    for (int i = 0; i < 3; i++) {
        DrawTexture(cannonballTexture,
            cannon.ballStackBounds.x + 5,
            cannon.ballStackBounds.y + i * 20 + 5,
            WHITE);
    }

    // Draw ramrod
    DrawRectangleRec(cannon.ramrodBounds, BROWN);

    // Draw loading state
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
        cannon.bounds.y + 60,
        15,
        stateColor);

    // Draw dragging items
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
