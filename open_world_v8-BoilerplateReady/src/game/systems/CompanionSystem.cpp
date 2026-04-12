#include "game/systems/CompanionSystem.h"

#include "ecs/World.h"
#include "game/party/PartyRuntimeState.h"

namespace fw {

void CompanionSystem::Update(World& world, PartyRuntimeState& partyState, float dt) {
    (void)world;
    (void)partyState;
    (void)dt;
    // v39 scaffold: companion follow / assist behavior will be implemented here.
}

} // namespace fw
