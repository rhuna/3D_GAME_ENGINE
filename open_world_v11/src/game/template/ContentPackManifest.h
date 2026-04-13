#pragma once

#include <string>
#include <vector>

namespace fw {

struct ContentPackManifest {
    std::string id;
    std::string displayName;
    std::string regionId;
    std::vector<std::string> npcFiles;
    std::vector<std::string> questFiles;
    std::vector<std::string> encounterFiles;
    std::vector<std::string> lootFiles;
};

} // namespace fw
