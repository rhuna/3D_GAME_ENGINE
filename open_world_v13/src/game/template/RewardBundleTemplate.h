#pragma once

#include <string>
#include <vector>

namespace fw {

struct RewardEntryTemplate {
    std::string type;
    std::string value;
    int quantity = 1;
};

struct RewardBundleTemplate {
    std::string rewardId;
    std::string displayName;
    std::vector<RewardEntryTemplate> entries;
};

} // namespace fw
