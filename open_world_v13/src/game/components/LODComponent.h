#pragma once

#include <string>
#include <vector>

namespace fw {

struct LODLevelDefinition {
    std::string modelPath;
    float maxDistance = 0.0f;
};

struct LODComponent {
    std::string groupId;
    std::vector<LODLevelDefinition> levels;
    int activeLevel = 0;
    bool lockHighestDetail = false;
};

} // namespace fw
