#pragma once
#include <string>
#include <unordered_map>
#include "game/content/models/LootTableDefinition.h"
namespace fw { class LootTableDatabase { public: bool LoadFromDirectory(const std::string& dir); const auto& GetAll() const { return m; } private: std::unordered_map<std::string, LootTableDefinition> m; }; }
