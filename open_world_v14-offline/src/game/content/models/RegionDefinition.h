#pragma once

#include <string>
#include <vector>

namespace fw {

struct RegionLinkDefinition {
    std::string targetRegion;
    std::string anchorId;
};

struct RegionDefinition {
    std::string id;
    std::string displayName;
    std::string sceneFile;
    std::string ambientMusicId;
    std::string description;
    bool safeZone = false;
    std::vector<RegionLinkDefinition> links;
};

} // namespace fw
