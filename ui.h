#pragma once
#include "raylib.h"
#include <cmath>

class UI {
public:
    UI();
    void Update();
    void Draw();
    float GetSteering() const { return steering; }
    float GetThrottle() const { return throttle; }
    bool IsFirePressed() const { return firePressed; }

private:
    Rectangle wheelBounds;
    Rectangle leverBounds;
    Rectangle fireBounds;
    float steering;
    float throttle;
    bool firePressed;

    static float Clamp(float value, float min, float max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
};
