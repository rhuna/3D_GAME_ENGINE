#pragma once
#include <string>
#include <unordered_map>
#include "game/content/models/ItemDefinition.h"
namespace fw { class ItemDatabase { public: bool LoadFromDirectory(const std::string& dir); const auto& GetAll() const { return m; } const ItemDefinition* Find(const std::string& id) const; private: std::unordered_map<std::string, ItemDefinition> m; }; }
