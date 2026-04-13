#pragma once

#include <vector>

#include "raylib.h"

namespace fw {

struct PatrolComponent {
    std::vector<Vector3> points;
    int currentIndex = 0;
    float moveSpeed = 2.0f;
};

} // namespace fw
