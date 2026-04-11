#pragma once
#include <string>
#include "game/save/SaveGameProfile.h"
#include "game/content/ContentPipeline.h"
namespace fw { class QuestGameplaySystem { public: bool HasQuest(const SaveGameProfile& profile, const std::string& questId) const; void AcceptQuest(SaveGameProfile& profile, const ContentPipeline& pipeline, const std::string& questId) const; bool TryCompleteQuest(SaveGameProfile& profile, const ContentPipeline& pipeline, const std::string& questId) const; }; }
