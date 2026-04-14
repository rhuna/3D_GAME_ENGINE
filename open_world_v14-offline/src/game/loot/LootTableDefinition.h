#pragma once

#include <string>
#include <vector>

#include "game/loot/LootDrop.h"

namespace fw {

struct LootTableDefinition {
    std::string id;
    std::vector<LootDrop> drops;
};

} // namespace fw
