#pragma once
#include <string>
#include <vector>

namespace fw {

struct BranchingRewardOption {
    std::string conditionFlag;
    std::string rewardId;
};

struct BranchingRewardTemplate {
    std::string id;
    std::vector<BranchingRewardOption> options;
};

} // namespace fw
