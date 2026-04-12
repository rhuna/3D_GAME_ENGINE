#pragma once

#include <string>
#include <vector>

namespace fw {

enum class QuestRuntimeStatus {
    Inactive,
    Active,
    Completed,
    Failed
};

struct QuestObjectiveRuntimeState {
    std::string id;
    bool completed = false;
    int currentCount = 0;
    int targetCount = 0;
};

struct QuestRuntimeEntry {
    std::string id;
    QuestRuntimeStatus status = QuestRuntimeStatus::Inactive;
    std::vector<QuestObjectiveRuntimeState> objectives;
};

} // namespace fw
