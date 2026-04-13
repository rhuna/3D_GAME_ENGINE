#pragma once

#include <string>
#include <vector>

namespace fw {

struct ProgressionMilestoneTemplate {
    std::string milestoneId;
    std::string displayName;
    std::string description;
    std::vector<std::string> unlockTags;
};

} // namespace fw
