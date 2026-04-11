#pragma once

#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class PlayerControllerSystem : public IUpdateSystem {
public:
    const char* Name() const override { return "PlayerControllerSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;
};

} // namespace fw
