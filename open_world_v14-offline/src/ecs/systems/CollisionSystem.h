#pragma once

#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class CollisionSystem : public IFixedUpdateSystem {
public:
    const char* Name() const override { return "CollisionSystem"; }
    void FixedUpdate(Application& app, World& world, float fixedDeltaTime) override;
};

} // namespace fw
