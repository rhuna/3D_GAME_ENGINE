#pragma once
#include <string>
#include <vector>

namespace fw {

struct VerticalSliceObjective {
    std::string id;
    std::string description;
};

struct VerticalSliceManifest {
    std::string id;
    std::string displayName;
    std::string regionScene;
    std::string interiorScene;
    std::string starterQuestId;
    std::vector<VerticalSliceObjective> objectives;
};

} // namespace fw
