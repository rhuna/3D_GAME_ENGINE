#pragma once

#include "raylib.h"

namespace fw {

struct SpawnerComponent {
    Vector3 spawnPoint {0.0f, 0.0f, 0.0f};
    bool active = true;
};

} // namespace fw
