#pragma once

#include <string>
#include <vector>

namespace fw {

struct ExtensionManifest {
    std::vector<std::string> regionIds;
    std::vector<std::string> prefabIds;
    std::vector<std::string> questIds;
    std::vector<std::string> merchantIds;
    std::vector<std::string> factionIds;
    std::vector<std::string> companionIds;
};

} // namespace fw
