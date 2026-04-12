#include "game/systems/LootContainerSystem.h"

#include "core/Application.h"
#include "ecs/World.h"
#include "game/components/LootComponent.h"

namespace fw {

void LootContainerSystem::Update(Application&, World& world, float) {
    if (!m_state) return;
    for (Entity entity : world.Entities()) {
        LootComponent* loot = world.GetComponent<LootComponent>(entity);
        if (!loot || !loot->opened) continue;
        if (loot->destroyOnLoot) {
            world.DestroyEntity(entity);
        }
    }
}

} // namespace fw
