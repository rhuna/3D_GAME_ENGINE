#pragma once
#include <string>
#include <vector>

namespace fw {

struct MultiRegionSliceManifest {
    std::string id;
    std::vector<std::string> regionIds;
    std::vector<std::string> questIds;
    std::vector<std::string> linkIds;
};

} // namespace fw
