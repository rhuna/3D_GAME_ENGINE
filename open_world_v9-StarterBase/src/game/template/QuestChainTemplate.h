#pragma once
#include <string>
#include <vector>

namespace fw {

struct QuestChainStepTemplate {
    std::string id;
    std::string objectiveBundleId;
    std::string dialogueId;
    std::string rewardId;
};

struct QuestChainTemplate {
    std::string id;
    std::vector<QuestChainStepTemplate> steps;
    std::string completionFlag;
};

} // namespace fw
