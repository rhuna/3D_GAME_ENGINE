#pragma once

#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class RespawnSystem : public IUpdateSystem {
public:
    const char* Name() const override { return "RespawnSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;
};

} // namespace fw
