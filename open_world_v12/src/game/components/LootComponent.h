#pragma once

#include <string>
#include <vector>

namespace fw {

struct LootDrop {
    std::string itemId;
    std::string displayName;
    int quantity = 1;
};

struct LootComponent {
    std::vector<LootDrop> drops;
    bool opened = false;
    bool destroyOnLoot = false;
};

} // namespace fw
