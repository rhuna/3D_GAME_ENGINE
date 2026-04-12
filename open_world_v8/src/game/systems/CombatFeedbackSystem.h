#pragma once

#include "game/state/ArenaGameState.h"
#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class CombatFeedbackSystem : public IUpdateSystem {
public:
    explicit CombatFeedbackSystem(ArenaGameState* state) : m_state(state) {}

    const char* Name() const override { return "CombatFeedbackSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;

private:
    ArenaGameState* m_state = nullptr;
};

} // namespace fw
