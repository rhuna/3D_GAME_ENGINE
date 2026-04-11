#pragma once
#include <string>
#include <unordered_map>
#include <vector>
namespace fw { struct RoutineStep { float startHour=0.0f; float endHour=24.0f; std::string activity; std::string locationTag; }; struct RoutineDefinition { std::string id; std::vector<RoutineStep> steps; }; class RoutineDatabase { public: bool LoadFromDirectory(const std::string& dir); const std::unordered_map<std::string, RoutineDefinition>& GetAll() const { return m_routines; } const RoutineDefinition* Find(const std::string& id) const; private: std::unordered_map<std::string, RoutineDefinition> m_routines; }; }
