#pragma once

#include "game/state/OpenWorldGameState.h"
#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class EquipmentSystem : public IUpdateSystem {
public:
    explicit EquipmentSystem(OpenWorldGameState* state) : m_state(state) {}

    const char* Name() const override { return "EquipmentSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;

private:
    OpenWorldGameState* m_state = nullptr;
};

} // namespace fw
