#pragma once
#include <string>
#include <vector>

namespace fw {

struct FrameworkFreezeManifest {
    std::string baselineVersion = "v52";
    std::vector<std::string> frozenAreas;
    std::vector<std::string> nextFocusAreas;
};

} // namespace fw
