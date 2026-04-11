#pragma once
#include <string>
#include <unordered_map>
#include "game/content/models/QuestDefinition.h"
namespace fw { class QuestDatabase { public: bool LoadFromDirectory(const std::string& dir); const auto& GetAll() const { return m; } const QuestDefinition* Find(const std::string& id) const; private: std::unordered_map<std::string, QuestDefinition> m; }; }
