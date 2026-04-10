#pragma once

#include "game/state/ArenaGameState.h"

namespace fw {

class World;

class GameHud {
public:
    void Draw(const World& world, const ArenaGameState& state) const;
};

} // namespace fw
