#include "game/systems/GatheringNodeSystem.h"

#include "ecs/World.h"

namespace fw {

void GatheringNodeSystem::Update(World& world, float dt)
{
    (void)world;
    (void)dt;
    // v37 scaffold:
    // - tick gather node cooldowns
    // - restore harvested nodes after respawn
    // - emit pickup/loot events when interacted
}

} // namespace fw
