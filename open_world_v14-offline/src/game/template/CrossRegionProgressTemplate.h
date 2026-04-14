#pragma once
#include <string>
#include <vector>

namespace fw {

struct CrossRegionProgressStep {
    std::string regionId;
    std::string triggerId;
    std::string setsFlag;
};

struct CrossRegionProgressTemplate {
    std::string id;
    std::vector<CrossRegionProgressStep> steps;
};

} // namespace fw
