#pragma once

#include "game/state/ArenaGameState.h"
#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class PickupSystem : public IUpdateSystem {
public:
    explicit PickupSystem(ArenaGameState* state) : m_state(state) {}

    const char* Name() const override { return "PickupSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;

private:
    ArenaGameState* m_state = nullptr;
};

} // namespace fw
