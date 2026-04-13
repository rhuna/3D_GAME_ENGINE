#pragma once

#include "ecs/Entity.h"
#include "game/state/OpenWorldGameState.h"
#include "gameplay/registry/SystemRegistry.h"

namespace fw {

class InteractionSystem : public IUpdateSystem {
public:
    explicit InteractionSystem(OpenWorldGameState* state) : m_state(state) {}

    const char* Name() const override { return "InteractionSystem"; }
    void Update(Application& app, World& world, float deltaTime) override;

private:
    OpenWorldGameState* m_state = nullptr;
    Entity FindBestInteractable(const World& world, Entity player) const;
};

} // namespace fw
