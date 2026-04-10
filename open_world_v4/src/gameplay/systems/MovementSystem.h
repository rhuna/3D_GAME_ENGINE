#pragma once

#include "ecs/World.h"

namespace fw {

class MovementSystem {
public:
    void FixedUpdate(World& world, float fixedDeltaTime) const;
};

} // namespace fw
