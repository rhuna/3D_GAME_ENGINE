#include "game/systems/BuildingInteriorSystem.h"

#include "core/Application.h"
#include "ecs/World.h"
#include "game/world/BuildingInteriorRuntime.h"

namespace fw {

void BuildingInteriorSystem::Update(Application& app, World& world, BuildingInteriorState& state, float dt) {
    (void)app;
    (void)world;
    (void)state;
    (void)dt;
    // v35 scaffold:
    // - detect enter / exit triggers
    // - transition between exterior and interior scenes
    // - restore player position from anchor ids
    // - preserve per-building runtime state
}

} // namespace fw
