#include "game/systems/QuestSystem.h"

#include "core/Application.h"
#include "game/quests/QuestState.h"

namespace fw {

void QuestSystem::Update(Application&, World&, float) {
    if (!m_state) return;
    for (auto& quest : m_state->saveProfile.quests) {
        if (quest.state == QuestProgressState::Active) {
            bool complete = true;
            for (const auto& objective : quest.objectives) {
                if (!objective.completed) { complete = false; break; }
            }
            if (complete) {
                quest.state = QuestProgressState::Completed;
            }
        }
    }
}

} // namespace fw
