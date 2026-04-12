#pragma once

#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class PatrolSystem : public IUpdateSystem {
public:
    const char* Name() const override { return "PatrolSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;
};

} // namespace fw
