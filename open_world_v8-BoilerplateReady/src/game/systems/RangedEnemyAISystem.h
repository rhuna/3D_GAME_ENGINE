#pragma once

#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class RangedEnemyAISystem : public IUpdateSystem {
public:
    const char* Name() const override { return "RangedEnemyAISystem"; }
    void Update(Application& app, World& world, float deltaTime) override;
};

} // namespace fw
