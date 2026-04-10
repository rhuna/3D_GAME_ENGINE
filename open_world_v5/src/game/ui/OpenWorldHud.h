#pragma once

#include "ecs/World.h"
#include "game/state/OpenWorldGameState.h"

namespace fw {

class OpenWorldHud {
public:
    void Draw(const World& world, const OpenWorldGameState& state) const;
};

} // namespace fw
