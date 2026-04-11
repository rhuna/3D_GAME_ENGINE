#pragma once
#include <string>
#include <vector>

namespace fw
{
    enum class QuestProgressState { NotStarted = 0, Active = 1, Completed = 2 };

    struct QuestObjectiveState
    {
        std::string id;
        int current = 0;
        int required = 1;
        bool completed = false;
    };

    struct QuestEntry
    {
        std::string id;
        std::string title;
        QuestProgressState state = QuestProgressState::NotStarted;
        std::vector<QuestObjectiveState> objectives;
    };
}
