#pragma once

#include "game/state/OpenWorldGameState.h"

namespace fw {

class QuestLogWindow {
public:
    void Draw(const OpenWorldGameState& state) const;
};

} // namespace fw
