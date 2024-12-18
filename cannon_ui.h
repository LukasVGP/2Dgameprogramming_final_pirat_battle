#pragma once
#include "raylib.h"

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

class CannonLoadingUI {
public:
    CannonLoadingUI();
    void Update();
    void Draw();
    bool IsLeftCannonReady() const { return leftCannon.state == CannonState::READY_TO_FIRE; }
    bool IsRightCannonReady() const { return rightCannon.state == CannonState::READY_TO_FIRE; }
    void ResetCannons();

private:
    void UpdateCannonDragging(CannonUI& cannon);
    void DrawCannon(const CannonUI& cannon);

    CannonUI leftCannon;
    CannonUI rightCannon;
    Texture2D cannonballTexture;
    Texture2D ramrodTexture;
};
