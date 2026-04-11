#pragma once
#include <string>
#include <unordered_map>
#include "game/content/models/NpcDefinition.h"
namespace fw { class NpcDatabase { public: bool LoadFromDirectory(const std::string& dir); const auto& GetAll() const { return m; } const NpcDefinition* Find(const std::string& id) const; private: std::unordered_map<std::string, NpcDefinition> m; }; }
