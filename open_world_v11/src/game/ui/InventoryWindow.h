#pragma once

#include "game/state/OpenWorldGameState.h"

namespace fw {

class InventoryWindow {
public:
    void Draw(const OpenWorldGameState& state) const;
};

} // namespace fw
