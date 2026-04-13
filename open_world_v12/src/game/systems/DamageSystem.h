#pragma once

#include "gameplay/registry/SystemRegistry.h"

namespace fw {

struct ArenaGameState;

class DamageSystem : public IUpdateSystem {
public:
    explicit DamageSystem(ArenaGameState* state) : m_state(state) {}

    const char* Name() const override { return "DamageSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;

private:
    ArenaGameState* m_state = nullptr;
};

} // namespace fw
