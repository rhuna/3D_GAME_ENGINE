#pragma once

#include <string>

namespace fw {

struct GatherNodeComponent {
    std::string nodeId;
    std::string resourceId;
    int remainingUses = 1;
    float respawnSeconds = 300.0f;
    bool harvested = false;
};

} // namespace fw
