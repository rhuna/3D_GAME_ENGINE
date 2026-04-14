#pragma once
#include <string>
#include <unordered_map>
#include "game/content/models/DialogueDefinition.h"
namespace fw { class DialogueDatabase { public: bool LoadFromDirectory(const std::string& dir); const auto& GetAll() const { return m; } const DialogueDefinition* Find(const std::string& id) const; private: std::unordered_map<std::string, DialogueDefinition> m; }; }
