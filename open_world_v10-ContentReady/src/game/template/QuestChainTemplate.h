#pragma once
#include <string>   // Used for quest and step ids.
#include <vector>   // Used for the ordered list of steps.

namespace fw {

struct QuestChainStep {
    std::string id;          // Stable step id.
    std::string objectiveId; // Objective definition tied to this step.
    std::string dialogueId;  // Dialogue shown or unlocked at this step.
};

struct QuestChainTemplate {
    std::string id;                         // Unique chain id.
    std::vector<QuestChainStep> steps;      // Ordered quest progression steps.
    std::string completionRewardBundleId;   // Reward bundle granted when the chain finishes.
};

} // namespace fw
