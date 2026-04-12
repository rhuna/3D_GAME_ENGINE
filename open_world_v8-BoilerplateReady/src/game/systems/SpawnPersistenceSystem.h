#pragma once

#include "game/state/OpenWorldGameState.h"
#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class SpawnPersistenceSystem : public IUpdateSystem {
public:
    explicit SpawnPersistenceSystem(OpenWorldGameState* state) : m_state(state) {}

    const char* Name() const override { return "SpawnPersistenceSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;

private:
    OpenWorldGameState* m_state = nullptr;
};

} // namespace fw
