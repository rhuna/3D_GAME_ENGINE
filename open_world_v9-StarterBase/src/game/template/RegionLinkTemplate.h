#pragma once
#include <string>

namespace fw {

struct RegionLinkTemplate {
    std::string fromRegionId;
    std::string toRegionId;
    std::string entryMarkerId;
    std::string exitMarkerId;
};

} // namespace fw
