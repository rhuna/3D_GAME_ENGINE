#pragma once

#include <string>

namespace fw {

struct QuestGiverComponent {
    std::string npcId;
    std::string offeredQuestId;
    bool repeatable = false;
};

} // namespace fw
