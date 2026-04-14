#pragma once

#include <string>

namespace fw {

struct OpenWorldRuntimeState {
    std::string currentRegionId = "village";
    std::string pendingRegionId;
    bool regionTransitionRequested = false;
    bool saveRequested = false;
};

} // namespace fw
