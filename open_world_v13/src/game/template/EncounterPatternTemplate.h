#pragma once
#include <string>
#include <vector>

namespace fw {

struct EncounterPatternTemplate {
    std::string id;
    std::string faction;
    std::vector<std::string> enemyPrefabs;
    int recommendedLevel = 1;
};

} // namespace fw
