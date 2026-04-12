#pragma once
#include <string>
#include <vector>
#include "game/content/NpcDatabase.h"
#include "game/simulation/RoutineDatabase.h"
namespace fw { struct ActiveNpcRoutineInfo { std::string npcId; std::string npcName; std::string routineId; std::string activity; std::string locationTag; }; class NpcRoutineSystem { public: std::vector<ActiveNpcRoutineInfo> ResolveActiveRoutines(const NpcDatabase& npcDb, const RoutineDatabase& routineDb, float hour, const std::string& currentRegion) const; }; }
