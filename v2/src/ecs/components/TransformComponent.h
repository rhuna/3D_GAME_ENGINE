#pragma once

#include "raylib.h"
#include "raymath.h"

namespace fw {
struct TransformComponent {
    Vector3 position {0.0f, 0.0f, 0.0f};
    Vector3 rotationEuler {0.0f, 0.0f, 0.0f};
    Vector3 scale {1.0f, 1.0f, 1.0f};
};
}
