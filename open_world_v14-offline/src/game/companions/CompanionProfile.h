#pragma once

#include <string>
#include <vector>

namespace fw {

struct CompanionProfile {
    std::string id;
    std::string displayName;
    std::string homeRegionId;
    std::vector<std::string> startingItems;
    bool availableAtStart = false;
};

} // namespace fw
