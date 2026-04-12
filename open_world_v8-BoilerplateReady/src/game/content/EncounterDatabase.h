#pragma once
#include <string>
#include <unordered_map>
#include "game/content/models/EncounterDefinition.h"
namespace fw { class EncounterDatabase { public: bool LoadFromDirectory(const std::string& dir); const auto& GetAll() const { return m; } private: std::unordered_map<std::string, EncounterDefinition> m; }; }
