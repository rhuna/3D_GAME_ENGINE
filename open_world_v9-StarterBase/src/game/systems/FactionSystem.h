#pragma once

#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class FactionSystem : public IUpdateSystem {
public:
    const char* Name() const override { return "FactionSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;
};

} // namespace fw
