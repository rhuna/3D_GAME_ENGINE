#pragma once

#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class ProjectileCleanupSystem : public IFixedUpdateSystem {
public:
    const char* Name() const override { return "ProjectileCleanupSystem"; }
    void FixedUpdate(Application& app, World& world, float fixedDeltaTime) override;
};

} // namespace fw
