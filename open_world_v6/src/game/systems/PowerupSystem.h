#pragma once

#include "game/state/ArenaGameState.h"
#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class PowerupSystem : public IUpdateSystem {
public:
    explicit PowerupSystem(ArenaGameState* state) : m_state(state) {}

    const char* Name() const override { return "PowerupSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;

private:
    ArenaGameState* m_state = nullptr;
};

} // namespace fw
