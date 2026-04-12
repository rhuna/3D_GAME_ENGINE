#include "game/systems/RoamingEncounterSystem.h"

#include "core/Application.h"
#include "ecs/World.h"
#include "game/components/EncounterSpawnerComponent.h"

namespace fw {

void RoamingEncounterSystem::Update(Application&, World& world, float deltaTime) {
    for (Entity entity : world.Entities()) {
        auto* spawner = world.GetComponent<EncounterSpawnerComponent>(entity);
        if (!spawner) continue;
        spawner->cooldown -= deltaTime;
        if (spawner->cooldown < 0.0f) spawner->cooldown = 0.0f;
    }
    (void)m_state;
}

} // namespace fw
