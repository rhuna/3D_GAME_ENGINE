#pragma once
#include <string>
#include <vector>

namespace fw {

struct RegionSliceTemplate {
    std::string regionId;
    std::string displayName;
    std::vector<std::string> encounterIds;
    std::vector<std::string> merchantIds;
    std::vector<std::string> gatherNodeIds;
    std::vector<std::string> craftingStationIds;
};

} // namespace fw
