#pragma once

#include <string>
#include <vector>

namespace fw {

struct StarterQuestObjectiveTemplate {
    std::string id;
    std::string type;
    std::string targetId;
    int requiredCount = 1;
};

struct StarterQuestTemplate {
    std::string id;
    std::string title;
    std::string giverNpcId;
    std::vector<StarterQuestObjectiveTemplate> objectives;
    std::vector<std::string> rewardItemIds;
};

} // namespace fw
