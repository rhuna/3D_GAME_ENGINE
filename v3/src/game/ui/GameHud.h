#pragma once

#include "game/state/ArenaGameState.h"
#include "game/ui/EndScreen.h"
#include "game/ui/MainMenu.h"

namespace fw {

class World;

class GameHud {
public:
    void Draw(const World& world, const ArenaGameState& state) const;

private:
    MainMenu m_mainMenu {};
    EndScreen m_endScreen {};
};

} // namespace fw
