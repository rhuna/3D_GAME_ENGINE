#pragma once

#include "raylib.h"

namespace fw {
struct BoxColliderComponent {
    Vector3 halfExtents {0.5f, 0.5f, 0.5f};
    bool isTrigger = false;
    bool isStatic = false;
};
}
