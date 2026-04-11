#include "game/gameplay/QuestGameplaySystem.h"
#include <algorithm>

namespace fw
{
    bool QuestGameplaySystem::HasQuest(const SaveGameProfile& profile, const std::string& questId) const
    {
        for (const auto& q : profile.quests) if (q.id == questId) return true;
        return false;
    }

    void QuestGameplaySystem::AcceptQuest(SaveGameProfile& profile, const ContentPipeline& pipeline, const std::string& questId) const
    {
        if (HasQuest(profile, questId)) return;
        const auto* def = pipeline.quests.Find(questId);
        if (!def) return;

        QuestEntry q{};
        q.id = def->id;
        q.title = def->title;
        q.state = QuestProgressState::Active;
        q.objectives.push_back({"collect_" + def->targetItemId, 0, def->targetCount, false});
        profile.quests.push_back(q);
    }

    bool QuestGameplaySystem::TryCompleteQuest(SaveGameProfile& profile, const ContentPipeline& pipeline, const std::string& questId) const
    {
        const auto* def = pipeline.quests.Find(questId);
        if (!def) return false;

        auto it = std::find(profile.inventory.items.begin(), profile.inventory.items.end(), def->targetItemId);
        int count = static_cast<int>(std::count(profile.inventory.items.begin(), profile.inventory.items.end(), def->targetItemId));
        if (count < def->targetCount) return false;

        int removed = 0;
        profile.inventory.items.erase(
            std::remove_if(profile.inventory.items.begin(), profile.inventory.items.end(),
                [&](const std::string& item)
                {
                    if (item == def->targetItemId && removed < def->targetCount)
                    {
                        ++removed;
                        return true;
                    }
                    return false;
                }),
            profile.inventory.items.end());

        for (auto& q : profile.quests)
        {
            if (q.id == questId)
            {
                q.state = QuestProgressState::Completed;
                for (auto& obj : q.objectives)
                {
                    obj.current = obj.required;
                    obj.completed = true;
                }
            }
        }

        profile.inventory.gold += def->rewardGold;
        if (!def->rewardItemId.empty())
            profile.inventory.items.push_back(def->rewardItemId);
        return true;
    }
}
