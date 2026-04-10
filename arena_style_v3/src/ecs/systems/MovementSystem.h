#pragma once

#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class MovementSystem : public IFixedUpdateSystem {
public:
    const char* Name() const override { return "MovementSystem"; }
    void FixedUpdate(Application& app, World& world, float fixedDeltaTime) override;
};

} // namespace fw
