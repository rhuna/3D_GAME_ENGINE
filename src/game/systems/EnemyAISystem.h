#pragma once

#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class EnemyAISystem : public IUpdateSystem {
public:
    const char* Name() const override { return "EnemyAISystem"; }
    void Update(Application& app, World& world, float deltaTime) override;
};

} // namespace fw
