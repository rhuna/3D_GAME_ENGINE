#pragma once

#include "raylib.h"
#include "raymath.h"

namespace fw {
struct RigidbodyComponent {
    Vector3 velocity {0.0f, 0.0f, 0.0f};
    Vector3 acceleration {0.0f, 0.0f, 0.0f};
    float drag = 0.0f;
    bool useGravity = false;
    bool kinematic = false;
};
}
