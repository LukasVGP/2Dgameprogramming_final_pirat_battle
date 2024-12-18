#pragma once
#include "raylib.h"
#include <cmath>

enum class CannonState {
    EMPTY,
    LOADED_BALL,
    READY_TO_FIRE
};

struct CannonUI {
    Rectangle bounds;
    Rectangle ballStackBounds;
    Rectangle ramrodBounds;
    CannonState state;
    bool isLeftCannon;
    float rotation;
    bool isDraggingBall;
    bool isDraggingRamrod;
    Vector2 dragOffset;
};

class UI {
public:
    UI();
    void Update();
    void Draw();

    // Ship control getters
    float GetSteering() const { return steering; }
    float GetThrottle() const { return throttle; }
    bool IsFirePressed() const { return firePressed; }

    // Cannon state getters
    bool IsLeftCannonReady() const { return leftCannon.state == CannonState::READY_TO_FIRE; }
    bool IsRightCannonReady() const { return rightCannon.state == CannonState::READY_TO_FIRE; }
    void ResetCannons();

private:
    // Ship control members
    Rectangle wheelBounds;
    Rectangle leverBounds;
    Rectangle fireBounds;
    float steering;
    float throttle;
    bool firePressed;

    // Cannon members
    static constexpr float CANNON_SPACING = 150.0f;
    static constexpr float CANNON_WIDTH = 120.0f;
    static constexpr float CANNON_HEIGHT = 60.0f;
    static constexpr float INDICATOR_OFFSET = 20.0f;

    CannonUI leftCannon;
    CannonUI rightCannon;
    Texture2D cannonballTexture;
    Texture2D ramrodTexture;

    // Helper methods
    static float Clamp(float value, float min, float max);
    void UpdateCannonDragging(CannonUI& cannon);
    void DrawCannon(const CannonUI& cannon);
    void DrawShipControls();
    void DrawCannonControls();
};
