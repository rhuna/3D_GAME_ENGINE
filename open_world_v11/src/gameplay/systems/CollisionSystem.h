#pragma once

#include "ecs/World.h"

namespace fw {

class CollisionSystem {
public:
    void FixedUpdate(World& world) const;
};

} // namespace fw
