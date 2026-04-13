#pragma once

#include "game/state/OpenWorldGameState.h"
#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class InventorySystem : public IUpdateSystem {
public:
    explicit InventorySystem(OpenWorldGameState* state) : m_state(state) {}

    const char* Name() const override { return "InventorySystem"; }
    void Update(Application& app, World& world, float deltaTime) override;

private:
    OpenWorldGameState* m_state = nullptr;
};

} // namespace fw
