#pragma once

#include <string>
#include <unordered_set>

namespace fw {

struct WorldContentDelta {
    std::unordered_set<std::string> harvestedNodeIds;
    std::unordered_set<std::string> lootedContainerIds;
    std::unordered_set<std::string> defeatedEnemyIds;
    std::unordered_set<std::string> activatedSavePointIds;
};

} // namespace fw
