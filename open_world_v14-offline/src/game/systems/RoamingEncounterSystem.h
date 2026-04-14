#pragma once

#include "game/state/OpenWorldGameState.h"
#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class RoamingEncounterSystem : public IUpdateSystem {
public:
    explicit RoamingEncounterSystem(OpenWorldGameState* state) : m_state(state) {}

    const char* Name() const override { return "RoamingEncounterSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;

private:
    OpenWorldGameState* m_state = nullptr;
};

} // namespace fw
