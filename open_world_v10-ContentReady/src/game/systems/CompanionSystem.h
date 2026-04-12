#pragma once

namespace fw {

class World;
class PartyRuntimeState;

class CompanionSystem {
public:
    void Update(World& world, PartyRuntimeState& partyState, float dt);
};

} // namespace fw
