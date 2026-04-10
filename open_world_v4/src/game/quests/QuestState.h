#pragma once

#include <string>
#include <vector>

namespace fw {

enum class QuestStatus {
    NotStarted,
    Active,
    Completed
};

struct QuestObjective {
    std::string description;
    bool complete = false;
};

struct QuestEntry {
    std::string questId;
    std::string title;
    std::string description;
    QuestStatus status = QuestStatus::NotStarted;
    std::vector<QuestObjective> objectives;
};

} // namespace fw
